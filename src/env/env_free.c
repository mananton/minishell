/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 11:43:42 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 11:43:45 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Versão didática do env_free:
** Explica passo a passo a libertação da memória.
*/
void	env_free(t_env *env)
{
	size_t	i; /* índice para percorrer o vetor env->vars */

	/* 1) se a struct for NULL, não há nada para libertar */
	if (!env)
		return ;

	/* 2) se o vetor existir, libertamos cada string "KEY=VALUE" */
	if (env->vars)
	{
		i = 0;
		while (env->vars[i])
		{
			/* cada entrada foi alocada (ft_strdup/join_kv), então precisa de free */
			free(env->vars[i]);
			i++;
		}
		/* 3) libertamos o próprio vetor de ponteiros */
		free(env->vars);
	}

	/* 4) por último, libertamos a própria struct t_env */
	free(env);
}
