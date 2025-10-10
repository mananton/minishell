/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:51:29 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 14:18:51 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MS_BUILTIN_EXIT 1143

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <stdint.h>

/* ---------- env: nossa tabela KEY=VALUE ---------- */
typedef struct s_env
{
	char	**vars;
	int     last_status;  /* NOVO: status do último comando */
}			t_env;

typedef struct s_redir
{
	char	*in_file;    // nome do ficheiro após '<' ou NULL
	char	*out_file;   // nome do ficheiro após '>' ou '>>' ou NULL
	char	*heredoc_delim; // delimitador do heredoc ou NULL
	int		in_type;     // 0 = none, 1 = '<', 2 = '<<'
	int		out_type;    // 0 = none, 1 = '>', 2 = '>>'
	int		heredoc_expand; // 0 = literal (por agora)
}	t_redir;


/* --- pipeline structs --- */
typedef struct s_cmd
{
	char    **argv;    /* argv limpo (sem tokens de redir)            */
	t_redir  redir;    /* redireções do comando (se houver)           */
}	t_cmd;

# define TOKEN_META_QUOTED 0x1
# define HDOC_INTERRUPTED 130

/* --- pipeline parse --- */
int   split_pipeline(char **argv, char ****segs, int *count);
void  free_segments(char ****segs, int count);

/* --- comandos por segmento (usa parse_redirs já existente) --- */
int   build_cmdlist(char ***segs, int count, t_cmd **out_cmds);
void  free_cmdlist(t_cmd *cmds, int count);

/* --- execução do pipeline inteiro --- */
int   exec_pipeline(t_cmd *cmds, int n, t_env *env);

/* parse: retira tokens de redireção de argv e devolve argv “limpo” */
int		parse_redirs(char **argv_in, char ***argv_out, t_redir *r);
/* runtime: abrir fds conforme r (sem aplicar dup2) */
int		open_redirs(const t_redir *r, int *fd_in, int *fd_out, t_env *env);
/* runtime: para builtins → aplicar e restaurar stdio */
int		apply_redirs_parent(const t_redir *r, int *saved_in, int *saved_out,
			int *fd_in, int *fd_out, t_env *env);
void	restore_stdio_parent(int saved_in, int saved_out, int fd_in, int fd_out);

/* exec externo agora recebe redireções */
int		exec_external_redir(char **argv, t_env *env, const t_redir *r);

/* env/ */
t_env		*env_init(char **envp);
const char	*env_get(t_env *env, const char *key);
int			env_set(t_env *env, const char *key, const char *value);
void		env_free(t_env *env);
int			env_unset(t_env *env, const char *key); /* <- novo */
int			env_fix_shlvl(t_env *env);


/* utils/ */
void		put_str_fd(const char *s, int fd);
char		**split_simple_args(const char *line);
void		free_argv(char **argv);
char		*ft_itoa(int n);
int			ft_atoi_strict(const char *s, int *ok);


/* env utils (helpers) */
char		*ft_strdup(const char *s); /* <- novo protótipo */
int			key_matches(const char *entry, const char *key);
int			env_find_index(t_env *env, const char *key);
char		**env_grow(t_env *env, size_t add);
int			is_valid_ident(const char *s);

/* builtins/ */

int			is_builtin(const char *cmd);
int			run_builtin(char **argv, t_env *env);

int			builtin_pwd(t_env *env);
int			builtin_echo(t_env *env, char **argv);
int			builtin_cd(char **argv, t_env *env);
int			builtin_env(t_env *env, char **argv);
int			builtin_export(t_env *env, char **argv);
int			builtin_unset(t_env *env, char **argv); /* <- novo */
int     	builtin_exit(t_env *env, char **argv);
int         builtin_status(t_env *env, char **argv); /* <- novo */

/* --- parse (aspas simples) --- */

size_t  token_len(const char *s, size_t start, size_t *end);
int     split_count_tokens(const char *line, size_t *out_count);
char    *read_token(const char *s, size_t start, size_t end, size_t len,
			const char *status, size_t status_len, t_env *env);
char    **split_build_argv(const char *line, size_t count, t_env *env);
char    **split_args_quotes(const char *line, t_env *env);
size_t  tkn_advance(const char *s, size_t i, size_t *len, int *err);
void	token_meta_register(char *str, unsigned int flags);
unsigned int	token_meta_flags(const char *str);
void	token_meta_forget(char *str);

/* sinais */
void	signals_register_env(t_env *env);
void	signals_setup_interactive(void);
void	signals_setup_child(void);

/* --- exec --- */

char    *path_join_seg(const char *dir, size_t dir_len, const char *cmd);
char    *find_in_path(const char *cmd, t_env *env);
int     exec_external(char **argv, t_env *env);
char	*resolve_path(const char *cmd, t_env *env);
int	    exec_error_code(const char *path);	


#endif
