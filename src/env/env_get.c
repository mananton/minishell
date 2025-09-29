/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 11:04:40 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 11:05:54 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

// Como extraímos o VALUE de uma "KEY=VALUE".

const char	*env_get(t_env *env, const char *key)
{
	int		idx;   /* índice onde a KEY foi encontrada no vetor, ou -1   */
	size_t	klen;  /* tamanho da chave 'key' (ex.: "HOME" -> 4)          */

	/* 1) validação básica: precisamos de env e de uma key válida */
	if (!env || !key)
		return (NULL);

	/* 2) calcula o tamanho da key para depois pular "KEY=" */
	klen = strlen(key);

	/* 3) procura no vetor a posição da entrada "KEY=VALUE" */
	idx = env_find_index(env, key);
	if (idx < 0)
		return (NULL); /* não existe essa KEY na nossa tabela */

	/* 4) retorna um ponteiro para depois de "KEY=":
	**    endereço da string + tamanho da KEY + 1 (pular o '=') */
	return (env->vars[idx] + klen + 1);
}
