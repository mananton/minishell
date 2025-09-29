/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:51:29 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 11:12:39 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/* ---------- env: nossa tabela KEY=VALUE ---------- */
typedef struct s_env
{
	char	**vars;
}			t_env;

/* env/ */
t_env		*env_init(char **envp);
const char	*env_get(t_env *env, const char *key);
int			env_set(t_env *env, const char *key, const char *value);
void		env_free(t_env *env);
int			env_unset(t_env *env, const char *key); /* <- novo */

/* utils/ */
void		put_str_fd(const char *s, int fd);
char		**split_simple_args(const char *line);
void		free_argv(char **argv);

/* env utils (helpers) */
char		*ft_strdup(const char *s); /* <- novo protótipo */
int			key_matches(const char *entry, const char *key);
int			env_find_index(t_env *env, const char *key);
char		**env_grow(t_env *env, size_t add);

int			is_builtin(const char *cmd);
int			run_builtin(char **argv, t_env *env);

int			builtin_pwd(void);
int			builtin_echo(char **argv);
int			builtin_cd(char **argv, t_env *env);
int			builtin_env(t_env *env, char **argv);
int			builtin_export(t_env *env, char **argv);
int			builtin_unset(t_env *env, char **argv); /* <- novo */

#endif
