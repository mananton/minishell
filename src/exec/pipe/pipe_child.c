/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:03:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 15:03:42 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe_internal.h"

void	pipe_run_child_process(t_pipe_ctx *ctx, int index)
{
	t_child_ctx	child;
	int			fd_in;
	int			fd_out;
	int			res;

	child.idx = index;
	child.count = ctx->count;
	child.fds = ctx->fds;
	child.cmd = &ctx->cmds[index];
	child.env = ctx->env;
	if (pipe_prepare(&child) != 0)
		_exit(1);
	res = child_setup_io(&child, &fd_in, &fd_out);
	if (res != 0)
	{
		if (res == HDOC_INTERRUPTED)
			_exit(HDOC_INTERRUPTED);
		_exit(1);
	}
	exec_child_command(&child);
}

int	pipe_fork_children(t_pipe_ctx *ctx)
{
	int		index;
	pid_t	pid;

	index = 0;
	while (index < ctx->count)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("minishell: fork");
			return (1);
		}
		if (pid == 0)
			pipe_run_child_process(ctx, index);
		ctx->pids[index] = pid;
		index++;
	}
	return (0);
}
