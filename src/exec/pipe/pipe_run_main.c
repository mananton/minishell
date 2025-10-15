/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:04:53 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:13:13 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipe_internal.h"

int	exec_pipeline(t_cmd *cmds, int count, t_env *env)
{
	t_pipe_ctx	ctx;

	if (count <= 0)
		return (0);
	ctx.count = count;
	ctx.cmds = cmds;
	ctx.env = env;
	ctx.fds = NULL;
	ctx.pids = NULL;
	if (pipe_alloc_resources(&ctx) != 0)
		return (1);
	if (pipe_open_pipes(&ctx) != 0)
	{
		pipe_free_resources(&ctx);
		return (1);
	}
	if (pipe_fork_children(&ctx) != 0)
	{
		pipe_close_pipes_parent(&ctx);
		pipe_free_resources(&ctx);
		return (1);
	}
	return (pipe_finish_pipeline(&ctx));
}
