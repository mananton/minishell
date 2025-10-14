/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:51:29 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:38:55 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MS_BUILTIN_EXIT 1143

# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/* ---------- env: nossa tabela KEY=VALUE ---------- */
typedef struct s_export_mark
{
	char					*key;
	struct s_export_mark	*next;
}							t_export_mark;

typedef struct s_env
{
	char					**vars;
	int last_status; /* NOVO: status do último comando */
	t_export_mark			*export_marks;
}							t_env;

typedef struct s_redir_step
{
	int						kind;
	char					*word;
	int						flag;
}							t_redir_step;

typedef struct s_redir
{
	t_redir_step			*steps;
	size_t					count;
}							t_redir;

/* --- pipeline structs --- */
typedef struct s_cmd
{
	char **argv;   /* argv limpo (sem tokens de redir)            */
	t_redir redir; /* redireções do comando (se houver)           */
}							t_cmd;

typedef struct s_parent_redir
{
	int						saved_in;
	int						saved_out;
	int						fd_in;
	int						fd_out;
}							t_parent_redir;

typedef struct s_child_ctx
{
	int						idx;
	int						count;
	int (*fds)[2];
	t_cmd					*cmd;
	t_env					*env;
}							t_child_ctx;

typedef struct s_hd_ctx
{
	t_env					*env;
	const char				*status;
	int						*err;
}							t_hd_ctx;

typedef struct s_hd_scan
{
	size_t					i;
	size_t					total;
	int						in_single;
	int						in_double;
}							t_hd_scan;

typedef struct s_hd_emit
{
	size_t					i;
	size_t					j;
	int						in_single;
	int						in_double;
}							t_hd_emit;

typedef struct s_hd_task
{
	int						*fd_in;
	t_env					*env;
	const char				*status;
}							t_hd_task;

typedef struct s_hd_session
{
	t_hd_task				*task;
	const char				*delim;
	int						expand;
	int						pipefd[2];
	FILE					*saved_stream;
	struct sigaction		old;
}							t_hd_session;

typedef struct s_redir_state
{
	int						*fd_in;
	int						*fd_out;
	t_env					*env;
	char					**status;
}							t_redir_state;

typedef struct s_pipe_ctx
{
	int						count;
	int (*fds)[2];
	pid_t					*pids;
	t_cmd					*cmds;
	t_env					*env;
}							t_pipe_ctx;

typedef struct s_token_meta
{
	char					*str;
	unsigned int			flags;
	struct s_token_meta		*next;
}							t_token_meta;

typedef struct s_expand_ctx
{
	const char				*str;
	size_t					end;
	t_env					*env;
	const char				*status;
	size_t					status_len;
}							t_expand_ctx;

typedef struct s_quote_state
{
	int						in_single;
	int						in_double;
}							t_quote_state;

typedef struct s_emit
{
	char					*out;
	size_t					*pos;
}							t_emit;

typedef struct s_len_state
{
	size_t					index;
	size_t					count;
	t_quote_state			quotes;
}							t_len_state;

typedef struct s_copy_state
{
	size_t					index;
	t_quote_state			quotes;
	int						inhibit_glob;
}							t_copy_state;

typedef struct s_build_ctx
{
	const char				*line;
	size_t					count;
	t_env					*env;
	char					**argv;
	char					*status;
	size_t					status_len;
	size_t					index;
	size_t					arg_index;
}							t_build_ctx;

typedef struct s_token_build
{
	t_expand_ctx			ctx;
	t_copy_state			copy;
	t_emit					emit;
	size_t					out_pos;
}							t_token_build;

typedef struct s_len_runner
{
	t_expand_ctx			ctx;
	t_len_state				state;
	int						*err;
	int						*quoted_any;
}							t_len_runner;

typedef struct s_expand_node
{
	char			*value;
	int			own;
	struct s_expand_node	*next;
}t_expand_node;

# define TOKEN_META_QUOTED 0x1
# define TOKEN_META_NO_GLOB 0x2
# define HDOC_INTERRUPTED 130
# define REDIR_IN 1
# define REDIR_HEREDOC 2
# define REDIR_OUT_TRUNC 3
# define REDIR_OUT_APPEND 4

/* --- pipeline parse --- */
int							split_pipeline(char **argv, char ****segs,
								int *count);
void						free_segments(char ****segs, int count);

/* --- comandos por segmento (usa parse_redirs já existente) --- */
int							build_cmdlist(char ***segs, int count,
								t_cmd **out_cmds);
void						free_cmdlist(t_cmd *cmds, int count);

/* --- execução do pipeline inteiro --- */
int							exec_pipeline(t_cmd *cmds, int n, t_env *env);

/* parse: retira tokens de redireção de argv e devolve argv “limpo” */
int							parse_redirs(char **argv_in, char ***argv_out,
								t_redir *r);
/* runtime: abrir fds conforme r (sem aplicar dup2) */
int							open_redirs(const t_redir *r, int *fd_in,
								int *fd_out, t_env *env);
/* runtime: para builtins → aplicar e restaurar stdio */
int							apply_redirs_parent(const t_redir *r,
								t_parent_redir *io, t_env *env);
void						restore_stdio_parent(int saved_in, int saved_out,
								int fd_in, int fd_out);

/* exec externo agora recebe redireções */
int							exec_external_redir(char **argv, t_env *env,
								const t_redir *r);

/* env/ */
t_env						*env_init(char **envp);
const char					*env_get(t_env *env, const char *key);
int							env_set(t_env *env, const char *key,
								const char *value);
void						env_free(t_env *env);
int	env_unset(t_env *env, const char *key); /* <- novo */
int							env_fix_shlvl(t_env *env);

/* utils/ */
void						put_str_fd(const char *s, int fd);
char						**split_simple_args(const char *line);
void						free_argv(char **argv);
char						*ft_itoa(int n);
int							ft_atoi_strict(const char *s, int *ok);

/* env utils (helpers) */
char	*ft_strdup(const char *s); /* <- novo protótipo */
int							key_matches(const char *entry, const char *key);
int							env_find_index(t_env *env, const char *key);
char						**env_grow(t_env *env, size_t add);
int							is_valid_ident(const char *s);
void						export_mark_add(t_env *env, const char *key);
void						export_mark_remove(t_env *env, const char *key);
int							export_mark_has(t_env *env, const char *key);
void						export_mark_clear_all(t_env *env);

/* builtins/ */

int							is_builtin(const char *cmd);
int							run_builtin(char **argv, t_env *env);

int							builtin_pwd(t_env *env);
int							builtin_echo(t_env *env, char **argv);
int							builtin_cd(char **argv, t_env *env);
int							builtin_env(t_env *env, char **argv);
int							builtin_export(t_env *env, char **argv);
int	builtin_unset(t_env *env, char **argv); /* <- novo */
int							builtin_exit(t_env *env, char **argv);
int	builtin_status(t_env *env, char **argv); /* <- novo */

/* --- parse (aspas simples) --- */

size_t						token_len(const char *s, size_t start, size_t *end);
int							split_count_tokens(const char *line,
								size_t *out_count);
char						**split_build_argv(const char *line, size_t count,
								t_env *env);
char						**split_args_quotes(const char *line, t_env *env);
void						token_meta_register(char *str, unsigned int flags);
unsigned int				token_meta_flags(const char *str);
void						token_meta_forget(char *str);
int							expand_wildcards(char ***argv);
void						redir_clear(t_redir *r);

/* sinais */
void						signals_register_env(t_env *env);
void						signals_setup_interactive(void);
void						signals_setup_child(void);

/* --- exec --- */

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

#endif
