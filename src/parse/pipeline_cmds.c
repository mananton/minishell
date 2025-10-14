/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:09:33 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:26:49 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static void	redir_zero(t_redir *r)
{
	r->steps = NULL;
	r->count = 0;
}

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

void	free_cmdlist(t_cmd *cmds, int count)
{
	int	i;

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
