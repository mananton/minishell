/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:14:43 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:25:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static int	key_match(const char *entry, const char *key)
{
	size_t	i;

	if (!entry || !key)
		return (0);
	i = 0;
	while (key[i])
	{
		if (entry[i] != key[i])
			return (0);
		i++;
	}
	return (entry[i] == '=');
}

static size_t	find_index(const t_env *env, const char *key)
{
	size_t	i;

	if (!env || !env->vars || !key)
		return ((size_t) - 1);
	i = 0;
	while (env->vars[i])
	{
		if (key_match(env->vars[i], key))
			return (i);
		i++;
	}
	return ((size_t)-1);
}

static size_t	env_count(const t_env *env)
{
	size_t	n;

	n = 0;
	while (env && env->vars && env->vars[n])
		n++;
	return (n);
}

static void	remove_at(t_env *env, size_t idx)
{
	size_t	n;
	size_t	j;

	if (!env || !env->vars)
		return ;
	n = env_count(env);
	if (idx >= n)
		return ;
	free(env->vars[idx]);
	j = idx;
	while (j + 1 < n)
	{
		env->vars[j] = env->vars[j + 1];
		j++;
	}
	env->vars[n - 1] = NULL;
}

int	env_unset(t_env *env, const char *key)
{
	size_t	idx;

	if (!env || !key)
		return (1);
	idx = find_index(env, key);
	if (idx == (size_t)-1)
		return (0);
	remove_at(env, idx);
	export_mark_remove(env, key);
	return (0);
}
