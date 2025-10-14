/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:22:11 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:22:11 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_has_stdin(const t_redir *redir)
{
	size_t	i;

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
		{
			perror("minishell: dup2 <pipe");
			return (1);
		}
	}
	if (idx < n - 1)
	{
		if (dup2(fds[idx][1], STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2 >pipe");
			return (1);
		}
	}
	return (0);
}

static void	close_all_pipes(int n, int fds[][2])
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

int	pipe_prepare(const t_child_ctx *ctx)
{
	if (!ctx || !ctx->cmd)
		return (1);
	signals_setup_child();
	if (pipe_dups(ctx->idx, ctx->count, ctx->fds, &ctx->cmd->redir) != 0)
		return (1);
	close_all_pipes(ctx->count, ctx->fds);
	return (0);
}
