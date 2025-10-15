/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_context.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:03:02 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:13:04 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipe_internal.h"

int	pipe_alloc_resources(t_pipe_ctx *ctx)
{
	ctx->pids = (pid_t *)malloc(sizeof(pid_t) * ctx->count);
	if (!ctx->pids)
		return (1);
	ctx->fds = NULL;
	if (ctx->count > 1)
	{
		ctx->fds = (int (*)[2]) malloc(sizeof(int [2]) * (ctx->count - 1));
		if (!ctx->fds)
		{
			free(ctx->pids);
			ctx->pids = NULL;
			return (1);
		}
	}
	return (0);
}

int	pipe_open_pipes(t_pipe_ctx *ctx)
{
	int	i;

	if (ctx->count <= 1)
		return (0);
	i = 0;
	while (i < ctx->count - 1)
	{
		if (pipe(ctx->fds[i]) < 0)
		{
			perror("minishell: pipe");
			while (i > 0)
			{
				i--;
				close(ctx->fds[i][0]);
				close(ctx->fds[i][1]);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	pipe_close_pipes_parent(const t_pipe_ctx *ctx)
{
	int	i;

	if (!ctx->fds)
		return ;
	i = 0;
	while (i < ctx->count - 1)
	{
		close(ctx->fds[i][0]);
		close(ctx->fds[i][1]);
		i++;
	}
}

void	pipe_free_resources(t_pipe_ctx *ctx)
{
	if (ctx->fds)
	{
		free(ctx->fds);
		ctx->fds = NULL;
	}
	if (ctx->pids)
	{
		free(ctx->pids);
		ctx->pids = NULL;
	}
}
