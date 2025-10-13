/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 11:04:40 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:20:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*env_get(t_env *env, const char *key)
{
	int		idx;
	size_t	klen;

	if (!env || !key)
		return (NULL);
	klen = strlen(key);
	idx = env_find_index(env, key);
	if (idx < 0)
		return (NULL);
	return (env->vars[idx] + klen + 1);
}
