/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:22:00 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 12:22:02 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Versão didática do builtin_env:
** Explica passo a passo como listamos as variáveis.
*/
int	builtin_env(t_env *env, char **argv)
{
	size_t	i; /* índice para percorrer env->vars */

	/* 1) se não há env válido ou a tabela está ausente, não há o que listar */
	if (!env || !env->vars)
		return (1);

	/* 2) nesta fase, recusamos argumentos (implementação básica) */
	if (argv && argv[1])
		return (put_str_fd("minishell: env: too many arguments\n", 2), 1);

	/* 3) percorremos o vetor de "KEY=VALUE" até encontrar o NULL final
	**    e imprimimos cada linha exatamente como está (formato de ambiente) */
	i = 0;
	while (env->vars[i])
	{
		put_str_fd(env->vars[i], 1);
		put_str_fd("\n", 1);
		i++;
	}
	return (0);
}
