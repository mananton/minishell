/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:50:03 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:36:16 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "env/env.h"
# include "parse/pipeline.h"
# include "redir/redir.h"

typedef struct s_child_ctx
{
	int						idx;
	int						count;
	int						(*fds)[2];
	t_cmd					*cmd;
	t_env					*env;
}							t_child_ctx;

char						*path_join_seg(const char *dir, size_t dir_len,
								const char *cmd);
char						*find_in_path(const char *cmd, t_env *env);
int							exec_external(char **argv, t_env *env);
char						*resolve_path(const char *cmd, t_env *env);
int							exec_error_code(const char *path);

int							pipe_prepare(const t_child_ctx *ctx);
int							child_setup_io(const t_child_ctx *ctx, int *fd_in,
								int *fd_out);
void						exec_child_command(const t_child_ctx *ctx);

int							exec_external_redir(char **argv, t_env *env,
								const t_redir *r);

#endif
