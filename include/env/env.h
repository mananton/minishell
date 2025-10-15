/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:45:02 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:35:53 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stddef.h>

typedef struct s_export_mark
{
	char					*key;
	struct s_export_mark	*next;
}							t_export_mark;

typedef struct s_env
{
	char					**vars;
	int						last_status;
	t_export_mark			*export_marks;
}							t_env;

t_env						*env_init(char **envp);
const char					*env_get(t_env *env, const char *key);
int							env_set(t_env *env, const char *key,
								const char *value);
void						env_free(t_env *env);
int							env_unset(t_env *env, const char *key);
int							env_fix_shlvl(t_env *env);

int							key_matches(const char *entry, const char *key);
int							env_find_index(t_env *env, const char *key);
char						**env_grow(t_env *env, size_t add);

void						export_mark_add(t_env *env, const char *key);
void						export_mark_remove(t_env *env, const char *key);
int							export_mark_has(t_env *env, const char *key);
void						export_mark_clear_all(t_env *env);

#endif
