/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:58:50 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 13:56:44 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_uint_0_255(int n)
{
	int		i;
	char	buf[12];

	if (n < 0)
		n = 0;
	n &= 0xFF;
	buf[11] = '\0';
	i = 10;
	if (n == 0)
	{
		buf[i] = '0';
		put_str_fd(&buf[i], 1);
		put_str_fd("\n", 1);
		return ;
	}
	while (n > 0 && i >= 0)
	{
		buf[i] = '0' + (n % 10);
		n /= 10;
		i--;
	}
	put_str_fd(&buf[i + 1], 1);
	put_str_fd("\n", 1);
}

int	builtin_status(t_env *env, char **argv)
{
	(void)argv;
	if (!env)
		return (1);
	print_uint_0_255(env->last_status);
	return (0);
}
