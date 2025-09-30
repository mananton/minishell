/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:33:05 by mananton          #+#    #+#             */
/*   Updated: 2025/09/30 12:33:26 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** tkn_advance:
** - Se encontra aspa simples: consome até a aspa de fecho; soma ao len.
** - Se encontra aspa dupla:  idem (sem expansões/escapes).
** - Caso normal: conta 1 char e avança.
** - Em aspa não fechada: *err = 1.
** - Retorna o novo índice 'i' após consumir o trecho.
*/
size_t	tkn_advance(const char *s, size_t i, size_t *len, int *err)
{
	char	q;

	if (s[i] == '\'' || s[i] == '\"')
	{
		q = s[i++];
		while (s[i] && s[i] != q)
		{
			(*len)++;
			i++;
		}
		if (!s[i])
		{
			*err = 1;
			return (i);
		}
		return (i + 1); /* pula a aspa de fecho */
	}
	(*len)++;
	return (i + 1);
}
