/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:58:03 by mananton          #+#    #+#             */
/*   Updated: 2025/10/10 11:01:40 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <string.h>

static int	is_token_exact(char *s, const char *pattern)
{
	if (!s)
		return (0);
	if (strcmp(s, pattern) != 0)
		return (0);
	return ((token_meta_flags(s) & TOKEN_META_QUOTED) == 0);
}

static int	is_redir_operator(char *s)
{
	return (is_token_exact(s, "<") || is_token_exact(s, "<<")
		|| is_token_exact(s, ">") || is_token_exact(s, ">>"));
}

static void	redir_reset(t_redir *r)
{
	r->steps = NULL;
	r->count = 0;
}

static int	redir_append(t_redir *r, int kind, char *word, int flag)
{
	t_redir_step	*tmp;

	tmp = (t_redir_step *)malloc(sizeof(t_redir_step) * (r->count + 1));
	if (!tmp)
		return (1);
	if (r->steps)
	{
		memcpy(tmp, r->steps, sizeof(t_redir_step) * r->count);
		free(r->steps);
	}
	tmp[r->count].kind = kind;
	tmp[r->count].word = word;
	tmp[r->count].flag = flag;
	r->steps = tmp;
	r->count += 1;
	return (0);
}

/* conta quantos args vão sobrar sem tokens de redireção */
static size_t	count_argv_clean(char **av)
{
	size_t i;
	size_t keep;

	i = 0;
	keep = 0;
	while (av && av[i])
	{
		if (is_redir_operator(av[i]) && av[i + 1])
			i += 2;            /* pula par: '>'/ '>>' / '<' + ficheiro */
		else
		{
			keep++;
			i++;
		}
	}
	return (keep);
}

/* copia apenas os argumentos “reais” (sem redireções) para novo vetor */
static char	**build_clean_argv(char **av, size_t keep)
{
	char	**out;
	size_t	i;
	size_t	k;

	out = (char **)malloc(sizeof(char *) * (keep + 1));
	if (!out)
		return (NULL);
	i = 0;
	k = 0;
	while (av && av[i])
	{
		if (is_redir_operator(av[i]) && av[i + 1])
			i += 2;           /* salta tokens de redir */
		else
		{
			out[k] = ft_strdup(av[i]);
			if (!out[k])
			{
				out[k] = NULL;
				free_argv(out);
				return (NULL);
			}
			k++;
			i++;
		}
	}
	out[k] = NULL;
	return (out);
}

/* lê pares de redireção e preenche r; “última vence” */
static int	extract_redirs(char **av, t_redir *r)
{
	size_t	idx;
	int		flag;

	idx = 0;
	while (av && av[idx])
	{
		if (is_redir_operator(av[idx]) && !av[idx + 1])
			return (put_str_fd("minishell: syntax error: expected filename after redirection\n", 2), 1);
		if (is_token_exact(av[idx], "<<") && av[idx + 1])
		{
			flag = ((token_meta_flags(av[idx + 1]) & TOKEN_META_QUOTED) == 0);
			if (redir_append(r, REDIR_HEREDOC, av[idx + 1], flag) != 0)
				return (put_str_fd("minishell: malloc failed\n", 2), 1);
		}
		else if (is_token_exact(av[idx], "<") && av[idx + 1])
		{
			if (redir_append(r, REDIR_IN, av[idx + 1], 0) != 0)
				return (put_str_fd("minishell: malloc failed\n", 2), 1);
		}
		else if (is_token_exact(av[idx], ">") && av[idx + 1])
		{
			if (redir_append(r, REDIR_OUT_TRUNC, av[idx + 1], 0) != 0)
				return (put_str_fd("minishell: malloc failed\n", 2), 1);
		}
		else if (is_token_exact(av[idx], ">>") && av[idx + 1])
		{
			if (redir_append(r, REDIR_OUT_APPEND, av[idx + 1], 0) != 0)
				return (put_str_fd("minishell: malloc failed\n", 2), 1);
		}
		if (is_redir_operator(av[idx]) && av[idx + 1])
			idx += 2;
		else
			idx++;
	}
	return (0);
}

/* API: separa redireções e devolve argv “limpo” (duplica os argumentos úteis) */
int	parse_redirs(char **argv_in, char ***argv_out, t_redir *r)
{
	size_t keep;

	if (!argv_in || !argv_in[0] || !argv_out || !r)
		return (1);
	*argv_out = NULL;
	redir_reset(r);
	if (extract_redirs(argv_in, r) != 0)
	{
		redir_clear(r);
		return (1);
	}
	keep = count_argv_clean(argv_in);
	*argv_out = build_clean_argv(argv_in, keep);
	if (!*argv_out)
{
		redir_clear(r);
		return (put_str_fd("minishell: malloc failed\n", 2), 1);
}
	return (0);
}

void	redir_clear(t_redir *r)
{
	if (!r)
		return ;
	free(r->steps);
	r->steps = NULL;
	r->count = 0;
}
