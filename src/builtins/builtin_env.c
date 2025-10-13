/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:22:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 13:53:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_env *env, char **argv)
{
	size_t	i;

	if (!env || !env->vars)
		return (1);
	if (argv && argv[1])
		return (put_str_fd("minishell: env: too many arguments\n", 2), 1);
	i = 0;
	while (env->vars[i])
	{
		put_str_fd(env->vars[i], 1);
		put_str_fd("\n", 1);
		i++;
	}
	return (0);
}
