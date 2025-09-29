/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_strict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:09:38 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 12:09:40 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ft_atoi_strict:
** Converte string s para int (apenas números positivos).
**
** - Se a string for composta apenas por dígitos → retorna o número e *ok=1.
** - Se houver qualquer caractere inválido ou string vazia → retorna 0 e *ok=0.
*/
int	ft_atoi_strict(const char *s, int *ok)
{
	int	result;
	int	i;

	if (!s || !*s)   // string nula ou vazia
	{
		*ok = 0;
		return (0);
	}
	result = 0;
	i = 0;
	while (s[i])
	{
		// Se não estiver entre '0' e '9', então não é número
		if (s[i] < '0' || s[i] > '9')
		{
			*ok = 0;   // marca como erro
			return (0); // devolve 0
		}
		// Acumula o valor
		result = result * 10 + (s[i] - '0');
		i++;
	}
	*ok = 1;  // sucesso
	return (result);
}
