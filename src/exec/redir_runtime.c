/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_runtime.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:59:32 by mananton          #+#    #+#             */
/*   Updated: 2025/10/10 10:39:24 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>   /* open flags */
#include <unistd.h>  /* dup, dup2, close, pipe, write */
#include <stdio.h>   /* perror */
#include <string.h>  /* strcmp, strlen */
#include <signal.h>

static volatile sig_atomic_t	g_hd_sigint = 0;

static int	write_line_fd(int fd, const char *line)
{
	size_t	len;

	len = strlen(line);
	if (write(fd, line, len) < 0)
		return (perror("minishell: write"), 1);
	if (write(fd, "\n", 1) < 0)
		return (perror("minishell: write"), 1);
	return (0);
}

static void	heredoc_sigint(int sig)
{
	(void)sig;
	g_hd_sigint = 1;
	rl_replace_line("", 0);
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
}

static int	is_var_start(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (c == '_');
}

static int	is_var_char(char c)
{
	if (is_var_start(c))
		return (1);
	return (c >= '0' && c <= '9');
}

static size_t	hd_var_len(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] && is_var_char(s[len]))
		len++;
	return (len);
}

static const char	*hd_lookup_env(t_env *env, const char *name, size_t len,
			int *err)
{
	char		*dup;
	const char	*value;

	if (!env)
		return (NULL);
	dup = (char *)malloc(len + 1);
	if (!dup)
	{
		if (err)
			*err = 1;
		return (NULL);
	}
	memcpy(dup, name, len);
	dup[len] = '\0';
	value = env_get(env, dup);
	free(dup);
	return (value);
}

static size_t	hd_dollar_len(const char *s, size_t *consumed, t_env *env,
			const char *status, int *err)
{
	const char	*value;
	size_t		name_len;

	if (s[1] == '?')
	{
		*consumed = 2;
		return (status ? strlen(status) : 0);
	}
	if (!s[1] || !is_var_start(s[1]))
	{
		*consumed = 1;
		return (1);
	}
	name_len = hd_var_len(s + 1);
	value = hd_lookup_env(env, s + 1, name_len, err);
	*consumed = 1 + name_len;
	if (err && *err)
		return (0);
	if (!value)
		return (0);
	return (strlen(value));
}

static int	hd_copy_value(char *dst, size_t *j, const char *value)
{
	size_t	k;

	if (!value)
		return (0);
	k = 0;
	while (value[k])
	{
		dst[*j] = value[k];
		*j += 1;
		k++;
	}
	return (0);
}

static char	*expand_heredoc_line(const char *line, t_env *env,
			const char *status)
{
	size_t	i;
	size_t	j;
	size_t	total;
	size_t	consumed;
	int		in_single;
	int		in_double;
	int		err;
	char	*out;

	i = 0;
	total = 0;
	in_single = 0;
	in_double = 0;
	err = 0;
	while (line[i] && !err)
	{
		if (!in_double && line[i] == '\'')
		{
			in_single = !in_single;
			total++;
			i++;
		}
		else if (!in_single && line[i] == '"')
		{
			in_double = !in_double;
			total++;
			i++;
		}
		else if (line[i] == '$' && !in_single)
		{
			total += hd_dollar_len(line + i, &consumed, env, status, &err);
			i += consumed;
		}
		else
		{
			total++;
			i++;
		}
	}
	if (err)
		return (NULL);
	out = (char *)malloc(total + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (!in_double && line[i] == '\'')
		{
			in_single = !in_single;
			out[j++] = line[i++];
		}
		else if (!in_single && line[i] == '"')
		{
			in_double = !in_double;
			out[j++] = line[i++];
		}
		else if (line[i] == '$' && !in_single)
		{
			if (line[i + 1] == '?')
			{
				hd_copy_value(out, &j, status);
				i += 2;
			}
			else if (!line[i + 1] || !is_var_start(line[i + 1]))
			{
				out[j++] = line[i++];
			}
			else
			{
				size_t		name_len;
				const char	*value;

				name_len = hd_var_len(line + i + 1);
				value = hd_lookup_env(env, line + i + 1, name_len, &err);
				if (err)
					return (free(out), NULL);
				hd_copy_value(out, &j, value);
				i += 1 + name_len;
			}
		}
		else
			out[j++] = line[i++];
	}
	out[j] = '\0';
	return (out);
}

static int	handle_heredoc(const t_redir *r, int *fd_in, t_env *env,
			const char *status)
{
	int		pipefd[2];
	char	*line;
	char	*expanded;
	FILE	*saved_stream;
	struct sigaction	sa;
	struct sigaction	old;
	int					res;

	if (!r->heredoc_delim)
		return (0);
	if (pipe(pipefd) < 0)
		return (perror("minishell: pipe"), 1);
	g_hd_sigint = 0;
	saved_stream = rl_outstream;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint;
	if (sigaction(SIGINT, &sa, &old) < 0)
	{
		rl_outstream = saved_stream;
		close(pipefd[0]);
		close(pipefd[1]);
		return (perror("minishell: sigaction"), 1);
	}
	rl_outstream = stderr;
	res = 0;
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, r->heredoc_delim) == 0)
			break ;
		expanded = NULL;
		if (r->heredoc_expand && env)
		{
			expanded = expand_heredoc_line(line, env, status);
			if (!expanded)
			{
				res = 1;
				free(line);
				break ;
			}
		}
		if (write_line_fd(pipefd[1], expanded ? expanded : line) != 0)
		{
			res = 1;
			free(expanded);
			free(line);
			break ;
		}
		free(expanded);
		free(line);
	}
	if (line)
		free(line);
	rl_outstream = saved_stream;
	sigaction(SIGINT, &old, NULL);
	if (g_hd_sigint)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (HDOC_INTERRUPTED);
	}
	close(pipefd[1]);
	if (res != 0)
	{
		close(pipefd[0]);
		return (1);
	}
	*fd_in = pipefd[0];
	return (0);
}
/*
** abre fds conforme r; devolve 0 se ok, 1 se erro
** - fd_in / fd_out ficam -1 quando não usados
** - in_type:  0 = none, 1 = '<', 2 = '<<'
** - out_type: 0 = none, 1 = '>', 2 = '>>'
*/
int	open_redirs(const t_redir *r, int *fd_in, int *fd_out, t_env *env)
{
	int	flags;
	char	*status;
	int		hd_res;

	*fd_in = -1;
	*fd_out = -1;
	if (!r)
		return (0);
	if (r->in_type == 2)
	{
		status = ft_itoa(env ? env->last_status : 0);
		if (!status)
			return (put_str_fd("minishell: malloc failed\n", 2), 1);
		hd_res = handle_heredoc(r, fd_in, env, status);
		free(status);
		if (hd_res != 0)
			return (hd_res);
	}
	else if (r->in_type == 1 && r->in_file)
	{
		*fd_in = open(r->in_file, O_RDONLY);
		if (*fd_in < 0)
			return (perror("minishell: open <"), 1);
	}
	if (r->out_type != 0 && r->out_file)
	{
		flags = O_WRONLY | O_CREAT;
		if (r->out_type == 1)
			flags = flags | O_TRUNC;
		else if (r->out_type == 2)
			flags = flags | O_APPEND;
		*fd_out = open(r->out_file, flags, 0644);
		if (*fd_out < 0)
			return (perror("minishell: open >"), 1);
	}
	return (0);
}

/* salva backups de STDIN/STDOUT no processo pai */
int	save_stdio_parent(int *saved_in, int *saved_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in < 0 || *saved_out < 0)
	{
		perror("minishell: dup");
		if (*saved_in >= 0)
			close(*saved_in);
		if (*saved_out >= 0)
			close(*saved_out);
		return (1);
	}
	return (0);
}

/* aplica a redireção de entrada no pai; faz rollback local em erro */
static int	install_in_parent(int saved_in, int saved_out, int *fd_in, int *fd_out)
{
	if (*fd_in >= 0)
	{
		if (dup2(*fd_in, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 <");
			if (*fd_in >= 0)
				close(*fd_in);
			if (*fd_out >= 0)
				close(*fd_out);
			close(saved_in);
			close(saved_out);
			return (1);
		}
		close(*fd_in);
		*fd_in = -1;
	}
	return (0);
}

/* aplica a redireção de saída no pai; faz rollback local em erro */
static int	install_out_parent(int saved_in, int saved_out, int *fd_in, int *fd_out)
{
	if (*fd_out >= 0)
	{
		if (dup2(*fd_out, STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2 >");
			if (*fd_in >= 0)
				close(*fd_in);
			if (*fd_out >= 0)
				close(*fd_out);
			close(saved_in);
			close(saved_out);
			return (1);
		}
		close(*fd_out);
		*fd_out = -1;
	}
	return (0);
}

/*
** aplica dup2 dos fds temporários para STDIN/STDOUT (dividido em 2 passos)
*/
int	install_redirs_parent(int saved_in, int saved_out, int *fd_in, int *fd_out)
{
	if (install_in_parent(saved_in, saved_out, fd_in, fd_out) != 0)
		return (1);
	if (install_out_parent(saved_in, saved_out, fd_in, fd_out) != 0)
		return (1);
	return (0);
}

/*
** parent (para builtins):
** - salva STDIN/STDOUT
** - abre redireções
** - instala dup2 (entrada e saída) em duas funções curtas
*/
int	apply_redirs_parent(const t_redir *r,
			int *saved_in, int *saved_out, int *fd_in, int *fd_out, t_env *env)
{
	int	res;

	if (save_stdio_parent(saved_in, saved_out) != 0)
		return (1);
	res = open_redirs(r, fd_in, fd_out, env);
	if (res != 0)
	{
		if (*saved_in >= 0)
			close(*saved_in);
		if (*saved_out >= 0)
			close(*saved_out);
		return (res);
	}
	if (install_redirs_parent(*saved_in, *saved_out, fd_in, fd_out) != 0)
		return (1);
	return (0);
}

/*
** desfaz redireções no parent:
** - restaura STDIN/STDOUT
** - fecha backups e fds temporários remanescentes
*/
void	restore_stdio_parent(int saved_in, int saved_out, int fd_in, int fd_out)
{
	if (saved_in >= 0)
		dup2(saved_in, STDIN_FILENO);
	if (saved_out >= 0)
		dup2(saved_out, STDOUT_FILENO);
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
	if (fd_in >= 0)
		close(fd_in);
	if (fd_out >= 0)
		close(fd_out);
}
