/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:09:33 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 14:09:40 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/parse/pipeline_cmds.c */
#include "minishell.h"
#include <stdlib.h>

/* zera um t_redir (ajuste se tua struct tiver mais campos) */
static void	redir_zero(t_redir *r)
{
	r->steps = NULL;
	r->count = 0;
}

/* prepara UM comando a partir de um segmento: parse_redirs -> argv limpo + redirs
   retorna 0 em sucesso; 1 em erro */
static int	prepare_one_cmd(char **seg, t_cmd *dst)
{
	char	**clean;
	t_redir	r;

	redir_zero(&r);
	clean = NULL;
	if (parse_redirs(seg, &clean, &r) != 0)
	{
		redir_clear(&r);
		return (1);
	}
	if (!clean || !clean[0])
	{
		free(clean);
		redir_clear(&r);
		return (1);
	}
	dst->argv = clean;
	dst->redir = r;
	return (0);
}

/* para cada segmento, cria um t_cmd (argv limpo + redirs) */
int	build_cmdlist(char ***segs, int count, t_cmd **out_cmds)
{
	int		i;
	t_cmd	*cmds;

	*out_cmds = NULL;
	cmds = (t_cmd *)malloc(sizeof(t_cmd) * count);
	if (!cmds)
		return (1);
	i = 0;
	while (i < count)
	{
		if (prepare_one_cmd(segs[i], &cmds[i]) != 0)
		{
			/* limpa o que já foi construído */
			while (--i >= 0)
			{
				free_argv(cmds[i].argv);
				redir_clear(&cmds[i].redir);
			}
			free(cmds);
			return (1);
		}
		i++;
	}
	*out_cmds = cmds;
	return (0);
}

/* liberta a lista de comandos (argv limpo + redirs) */
void	free_cmdlist(t_cmd *cmds, int count)
{
	int i;

	if (!cmds)
		return ;
	i = 0;
	while (i < count)
	{
		free_argv(cmds[i].argv);
		redir_clear(&cmds[i].redir);
		i++;
	}
	free(cmds);
}
