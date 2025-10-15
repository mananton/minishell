/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:30:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:19:50 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_INTERNAL_H
# define REDIR_INTERNAL_H

# include "../../signals/signals_internal.h"
# include "env/env.h"
# include "redir/redir.h"
# include <signal.h>
# include <stddef.h>
# include <stdio.h>

typedef struct s_hd_ctx
{
	t_env				*env;
	const char			*status;
	int					*err;
}						t_hd_ctx;

typedef struct s_hd_scan
{
	size_t				i;
	size_t				total;
	int					in_single;
	int					in_double;
}						t_hd_scan;

typedef struct s_hd_emit
{
	size_t				i;
	size_t				j;
	int					in_single;
	int					in_double;
}						t_hd_emit;

typedef struct s_hd_task
{
	int					*fd_in;
	t_env				*env;
	const char			*status;
}						t_hd_task;

typedef struct s_hd_session
{
	t_hd_task			*task;
	const char			*delim;
	int					expand;
	int					pipefd[2];
	FILE				*saved_stream;
	struct sigaction	old;
}						t_hd_session;

typedef struct s_redir_state
{
	int					*fd_in;
	int					*fd_out;
	t_env				*env;
	char				**status;
}						t_redir_state;

int						write_line_fd(int fd, const char *line);
void					heredoc_sigint(int sig);
int						hd_open_pipe(t_hd_session *sess);
int						hd_enable_signal(t_hd_session *sess);
void					hd_restore_signal(t_hd_session *sess);

void					hd_close_pipe(int pipefd[2]);
void					hd_init_session(t_hd_session *sess, t_hd_task *task,
							const char *delim, int expand);
int						hd_session_loop(t_hd_session *sess);
int						hd_finalize_session(t_hd_session *sess, t_hd_task *task,
							int result);

int						is_var_start(char c);
int						is_var_char(char c);
size_t					hd_var_len(const char *s);
const char				*hd_lookup_env(t_env *env, const char *name, size_t len,
							int *err);
int						hd_copy_value(char *dst, size_t *j, const char *value);
size_t					hd_dollar_len(const char *s, t_hd_ctx *ctx,
							size_t *consumed);
int						hd_scan_single(const char *line, t_hd_scan *scan);
int						hd_scan_double(const char *line, t_hd_scan *scan);
int						hd_scan_dollar(const char *line, t_hd_scan *scan,
							t_hd_ctx *ctx);
size_t					hd_total_len(const char *line, t_hd_ctx *ctx);
size_t					hd_copy_dollar(const char *s, char *dst, size_t *j,
							t_hd_ctx *ctx);
int						hd_emit_single(const char *line, char *out,
							t_hd_emit *emit);
int						hd_emit_double(const char *line, char *out,
							t_hd_emit *emit);
int						hd_emit_dollar(const char *line, char *out,
							t_hd_emit *emit, t_hd_ctx *ctx);
void					hd_fill_output(const char *line, char *out,
							t_hd_ctx *ctx);
char					*expand_heredoc_line(const char *line, t_env *env,
							const char *status);

int						hd_ensure_status(char **status, t_env *env);
int						handle_heredoc(const t_redir_step *step,
							t_hd_task *task);
int						redir_run_heredoc(const t_redir_step *step,
							t_redir_state *st);

void					redir_close_fd(int *fd);
void					redir_assign_fd(int *fd, int new_fd);
int						redir_open_input(const char *path, int *fd_in);
int						redir_open_output(const t_redir_step *step,
							int *fd_out);
int						redir_dispatch(const t_redir_step *step,
							t_redir_state *st);
int						redir_finish(int result, int *fd_in, int *fd_out);

void					redir_close_saved(t_parent_redir *io);
int						save_stdio_parent(int *saved_in, int *saved_out);
int						install_redirs_parent(int saved_in, int saved_out,
							int *fd_in, int *fd_out);

#endif
