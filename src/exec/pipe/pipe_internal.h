/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:02:11 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:25:19 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_INTERNAL_H
# define PIPE_INTERNAL_H

# include "env/env.h"
# include "parse/pipeline.h"
# include <sys/types.h>

typedef struct s_pipe_ctx
{
	int		count;
	int		(*fds)[2];
	pid_t	*pids;
	t_cmd	*cmds;
	t_env	*env;
}			t_pipe_ctx;

int			pipe_alloc_resources(t_pipe_ctx *ctx);
int			pipe_open_pipes(t_pipe_ctx *ctx);
void		pipe_close_pipes_parent(const t_pipe_ctx *ctx);
void		pipe_free_resources(t_pipe_ctx *ctx);
void		pipe_run_child_process(t_pipe_ctx *ctx, int index);
int			pipe_fork_children(t_pipe_ctx *ctx);
void		pipe_handle_child_status(const t_pipe_ctx *ctx, int index,
				int status, int *last);
int			pipe_wait_children(const t_pipe_ctx *ctx);
int			pipe_finish_pipeline(t_pipe_ctx *ctx);

#endif
