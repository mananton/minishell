/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_finish.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:04:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 15:04:21 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe_internal.h"

void	pipe_handle_child_status(const t_pipe_ctx *ctx, int index, int status,
		int *last)
{
	const t_cmd	*cmd;

	if (index < ctx->count - 1 && WIFSIGNALED(status)
		&& WTERMSIG(status) == SIGPIPE)
	{
		cmd = &ctx->cmds[index];
		if (cmd->argv && cmd->argv[0])
		{
			put_str_fd(cmd->argv[0], 2);
			put_str_fd(": write error: Broken pipe\n", 2);
		}
	}
	if (index == ctx->count - 1)
	{
		if (WIFEXITED(status))
			*last = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*last = 128 + WTERMSIG(status);
		else
			*last = 1;
	}
}

int	pipe_wait_children(const t_pipe_ctx *ctx)
{
	int	index;
	int	status;
	int	last;

	index = 0;
	last = 0;
	while (index < ctx->count)
	{
		if (waitpid(ctx->pids[index], &status, 0) > 0)
			pipe_handle_child_status(ctx, index, status, &last);
		index++;
	}
	return (last);
}

int	pipe_finish_pipeline(t_pipe_ctx *ctx)
{
	int	status;

	pipe_close_pipes_parent(ctx);
	status = pipe_wait_children(ctx);
	pipe_free_resources(ctx);
	return (status);
}
