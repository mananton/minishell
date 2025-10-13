/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 11:26:26 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:22:49 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_kv(const char *key, const char *value)
{
	size_t	klen;
	size_t	vlen;
	char	*e;

	if (!key || !*key || !value)
		return (NULL);
	klen = strlen(key);
	vlen = strlen(value);
	e = (char *)malloc(klen + 1 + vlen + 1);
	if (!e)
		return (NULL);
	memcpy(e, key, klen);
	e[klen] = '=';
	memcpy(e + klen + 1, value, vlen);
	e[klen + 1 + vlen] = '\0';
	return (e);
}

static int	env_replace_existing(t_env *env, const char *key, char *entry)
{
	int	idx;

	idx = env_find_index(env, key);
	if (idx < 0)
		return (1);
	free(env->vars[idx]);
	env->vars[idx] = entry;
	export_mark_remove(env, key);
	return (0);
}

static int	env_append_entry(t_env *env, const char *key, char *entry)
{
	size_t	i;

	if (!env_grow(env, 1))
		return (free(entry), 1);
	i = 0;
	while (env->vars[i])
		i++;
	env->vars[i] = entry;
	env->vars[i + 1] = NULL;
	export_mark_remove(env, key);
	return (0);
}

int	env_set(t_env *env, const char *key, const char *value)
{
	char	*entry;

	if (!env || !key || !*key || !value)
		return (1);
	entry = join_kv(key, value);
	if (!entry)
		return (1);
	if (env_replace_existing(env, key, entry) == 0)
		return (0);
	return (env_append_entry(env, key, entry));
}
