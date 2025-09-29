/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ident.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:19:05 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 13:19:10 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* retorna 1 se c é letra ASCII */
static int	is_alpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

/* retorna 1 se c é letra, dígito ou '_' */
static int	is_alnum_uscore(char c)
{
	if (c == '_')
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (is_alpha(c));
}

/*
** is_valid_ident:
** - primeiro char: letra ou '_'
** - restante: letra/dígito/'_'
** - vazio: inválido
*/
int	is_valid_ident(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(is_alpha(*s) || *s == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!is_alnum_uscore(s[i]))
			return (0);
		i++;
	}
	return (1);
}
