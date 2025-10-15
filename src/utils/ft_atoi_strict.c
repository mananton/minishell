/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_strict.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:09:38 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 11:18:03 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoi_strict(const char *s, int *ok)
{
	int	result;
	int	i;

	if (!s || !*s)
	{
		*ok = 0;
		return (0);
	}
	result = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
		{
			*ok = 0;
			return (0);
		}
		result = result * 10 + (s[i] - '0');
		i++;
	}
	*ok = 1;
	return (result);
}
