/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:22:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:22:42 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	child_dup_fd(int *fd, int stdio, const char *who)
{
	if (*fd < 0)
		return (0);
	if (dup2(*fd, stdio) < 0)
	{
		perror(who);
		return (1);
	}
	close(*fd);
	*fd = -1;
	return (0);
}

static int	child_apply_redirs(const t_redir *redir, int *fd_in, int *fd_out,
			t_env *env)
{
	int	res;

	res = open_redirs(redir, fd_in, fd_out, env);
	if (res != 0)
		return (res);
	if (child_dup_fd(fd_in, STDIN_FILENO, "minishell: dup2 <") != 0)
		return (1);
	if (child_dup_fd(fd_out, STDOUT_FILENO, "minishell: dup2 >") != 0)
		return (1);
	return (0);
}

int	child_setup_io(const t_child_ctx *ctx, int *fd_in, int *fd_out)
{
	if (!ctx || !ctx->cmd || !fd_in || !fd_out)
		return (1);
	*fd_in = -1;
	*fd_out = -1;
	return (child_apply_redirs(&ctx->cmd->redir, fd_in, fd_out, ctx->env));
}
