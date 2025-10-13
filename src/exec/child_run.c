/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:27:22 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 14:27:24 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/exec/child_run.c */
#include "minishell.h"
#include <unistd.h>   /* dup2, close, execve, _exit */
#include <stdlib.h>   /* NULL */
#include <stdio.h>    /* perror */

/* forwards existentes noutros ficheiros */
char	*resolve_path(const char *cmd, t_env *env);
int		open_redirs(const t_redir *r, int *fd_in, int *fd_out, t_env *env);
int		exec_error_code(const char *path);

/* dup do pipe consoante posição no pipeline: idx em [0..n-1] */
static int	redir_has_stdin(const t_redir *redir)
{
	size_t i;

	if (!redir)
		return (0);
	i = 0;
	while (i < redir->count)
	{
		if (redir->steps[i].kind == REDIR_IN
			|| redir->steps[i].kind == REDIR_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

static int	pipe_dups(int idx, int n, int fds[][2], const t_redir *redir)
{
	if (idx > 0 && (!redir || !redir_has_stdin(redir)))
	{
		if (dup2(fds[idx - 1][0], STDIN_FILENO) < 0)
			return (perror("minishell: dup2 <pipe"), 1);
	}
	if (idx < n - 1)
	{
		if (dup2(fds[idx][1], STDOUT_FILENO) < 0)
			return (perror("minishell: dup2 >pipe"), 1);
	}
	return (0);
}

/* fecha todos os descritores de pipe neste processo (filho) */
static void	close_all_pipes(int n, int fds[][2])
{
	int i;

	i = 0;
	while (i < n - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

/* aplica redireções do comando (abre + dup2); devolve 0/erro */
static int	apply_redirs(const t_redir *r, int *fd_in, int *fd_out, t_env *env)
{
	int	res;

	res = open_redirs(r, fd_in, fd_out, env);
	if (res != 0)
		return (res);
	if (*fd_in >= 0)
	{
		if (dup2(*fd_in, STDIN_FILENO) < 0)
			return (perror("minishell: dup2 <"), 1);
		close(*fd_in);
		*fd_in = -1;
	}
	if (*fd_out >= 0)
	{
		if (dup2(*fd_out, STDOUT_FILENO) < 0)
			return (perror("minishell: dup2 >"), 1);
		close(*fd_out);
		*fd_out = -1;
	}
	return (0);
}

/* executa builtin no filho; respeita MS_BUILTIN_EXIT devolvendo _exit(code) */
static void	run_builtin_child(char **argv, t_env *env)
{
	int st;
	int code;

	st = run_builtin(argv, env);
	if (st >= MS_BUILTIN_EXIT)
	{
		code = (st - MS_BUILTIN_EXIT) & 0xFF;
		_exit(code);
	}
	_exit(st & 0xFF);
}

/* executa comando externo no filho (resolve PATH + execve) */
static void	run_external_child(char **argv, t_env *env)
{
	char		*path;
	char *const	*envp;

	path = resolve_path(argv[0], env);
	if (!path)
	{
		put_str_fd("minishell: command not found: ", 2);
		put_str_fd(argv[0], 2);
		put_str_fd("\n", 2);
		_exit(127);
	}
	envp = NULL;
	if (env)
		envp = (char *const *)env->vars;
	execve(path, argv, envp);
	_exit(exec_error_code(path));
}

/* filho: liga pipes, fecha fds, aplica redirs e executa builtin/externo */
int	run_child_cmd(int idx, int n, int fds[][2], t_cmd *cmd, t_env *env)
{
int	fd_in;
int	fd_out;
int	ar;

	signals_setup_child();
	if (pipe_dups(idx, n, fds, &cmd->redir) != 0)
		_exit(1);
	close_all_pipes(n, fds);
	fd_in = -1;
	fd_out = -1;
	ar = apply_redirs(&cmd->redir, &fd_in, &fd_out, env);
	if (ar != 0)
		_exit(ar == HDOC_INTERRUPTED ? HDOC_INTERRUPTED : 1);
	if (is_builtin(cmd->argv[0]))
		run_builtin_child(cmd->argv, env);
	run_external_child(cmd->argv, env);
	return (0);
}
