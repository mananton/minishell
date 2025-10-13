/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 11:43:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:19:03 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(t_env *env)
{
	size_t	i;

	if (!env)
		return ;
	export_mark_clear_all(env);
	if (env->vars)
	{
		i = 0;
		while (env->vars[i])
		{
			free(env->vars[i]);
			i++;
		}
		free(env->vars);
	}
	free(env);
}
