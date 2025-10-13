/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:38:04 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 13:54:23 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_init(char **envp)
{
	t_env	*env;
	size_t	i;
	size_t	count;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	count = 0;
	while (envp && envp[count])
		count++;
	env->vars = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env->vars)
		return (free(env), NULL);
	i = 0;
	while (envp && envp[i])
	{
		env->vars[i] = ft_strdup(envp[i]);
		if (!env->vars[i])
			return (env_free(env), NULL);
		i++;
	}
	env->vars[i] = NULL;
	env->last_status = 0;
	env->export_marks = NULL;
	return (env);
}
