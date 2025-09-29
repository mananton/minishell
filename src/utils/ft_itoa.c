/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:15:20 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 12:15:31 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** count_digits:
** Conta quantos dígitos tem o número.
*/
static int	count_digits(int n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

/*
** ft_itoa:
** Converte int positivo em string mallocada.
** - Retorna NULL se malloc falhar.
*/
char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	if (n < 0)
		return (NULL);
	len = count_digits(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len > 0)
	{
		str[len - 1] = (n % 10) + '0';
		n /= 10;
		len--;
	}
	return (str);
}
