/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:58:03 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 10:58:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/* zera a struct (sem alocar nada) */
static void	redir_reset(t_redir *r)
{
	r->in = NULL;
	r->out = NULL;
	r->append = 0;
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
		if ((av[i][0] == '>' || av[i][0] == '<') && av[i + 1])
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
		if ((av[i][0] == '>' || av[i][0] == '<') && av[i + 1])
			i += 2;           /* salta tokens de redir */
		else
			out[k++] = av[i++]; /* mantém */
	}
	out[k] = NULL;
	return (out);
}

/* lê pares de redireção e preenche r; “última vence” */
static int	extract_redirs(char **av, t_redir *r)
{
	size_t i;

	i = 0;
	while (av && av[i])
	{
		if (av[i][0] == '<' && av[i][1] == '\0' && av[i + 1])
			r->in = av[i + 1];
		else if (av[i][0] == '>' && av[i][1] == '\0' && av[i + 1])
		{
			r->out = av[i + 1];
			r->append = 0;
		}
		else if (av[i][0] == '>' && av[i][1] == '>' && av[i][2] == '\0' && av[i + 1])
		{
			r->out = av[i + 1];
			r->append = 1;
		}
		else if ((av[i][0] == '>' || av[i][0] == '<') && !av[i + 1])
			return (put_str_fd("minishell: syntax error: expected filename after redirection\n", 2), 1);
		i++;
	}
	return (0);
}

/* API: separa redireções e devolve argv “limpo” (sem alocar/copy strings) */
int	parse_redirs(char **argv_in, char ***argv_out, t_redir *r)
{
	size_t keep;

	if (!argv_in || !argv_in[0] || !argv_out || !r)
		return (1);
	redir_reset(r);
	if (extract_redirs(argv_in, r) != 0)
		return (1);
	keep = count_argv_clean(argv_in);
	*argv_out = build_clean_argv(argv_in, keep);
	if (!*argv_out)
		return (put_str_fd("minishell: malloc failed\n", 2), 1);
	return (0);
}
