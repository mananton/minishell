/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:58:50 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 14:18:42 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* imprime inteiro [0..255] em fd=1, com \n, sem depender de strlen fora do buffer */
static void print_uint_0_255(int n)
{
	char buf[12];      /* espaço de sobra */
	int  i;

	if (n < 0)
		n = 0;         /* só por segurança */
	n &= 0xFF;         /* normaliza 0..255 */

	buf[11] = '\0';    /* terminador */
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
	put_str_fd(&buf[i + 1], 1);  /* agora é string terminada */
	put_str_fd("\n", 1);
}

/* Imprime env->last_status e retorna 0 (o comando em si correu bem) */
int	builtin_status(t_env *env, char **argv)
{
	(void)argv;
	if (!env)
		return (1);
	print_uint_0_255(env->last_status);
	return (0);
}