/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:51:29 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 13:59:27 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MS_BUILTIN_EXIT 1143

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
char    *read_token(const char *s, size_t start, size_t end, size_t len);
char    **split_build_argv(const char *line, size_t count);
char    **split_args_quotes(const char *line);
size_t  tkn_advance(const char *s, size_t i, size_t *len, int *err);

/* --- exec --- */

char    *path_join_seg(const char *dir, size_t dir_len, const char *cmd);
char    *find_in_path(const char *cmd, t_env *env);
int     exec_external(char **argv, t_env *env);
char	*resolve_path(const char *cmd, t_env *env);


#endif