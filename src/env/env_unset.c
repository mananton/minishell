/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:14:43 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 11:14:47 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** env_unset:
** Remove a entrada para 'key' (se existir) de env->vars.
** - Procura índice com env_find_index.
** - Liberta a string.
** - Desloca elementos seguintes para a esquerda.
** - Mantém o terminador NULL no fim.
** Retorna 0 (sucesso) sempre; 1 só em erros óbvios de entrada.
*/
int	env_unset(t_env *env, const char *key)
{
	int		idx;
	size_t	i;

	if (!env || !env->vars || !key || !*key)
		return (1);
	idx = env_find_index(env, key);
	if (idx < 0)
		return (0);
	free(env->vars[idx]);
	i = (size_t)idx;
	while (env->vars[i + 1])
	{
		env->vars[i] = env->vars[i + 1];
		i++;
	}
	env->vars[i] = NULL;
	return (0);
}
