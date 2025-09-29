/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:49:38 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 13:49:40 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** builtin_exit (versão mínima):
** - Sem argumentos: imprime "exit" e sinaliza ao main para terminar.
** - Com argumentos: ainda não suportamos → erro e não sai.
*/
int	builtin_exit(t_env *env, char **argv)
{
	(void)env;
	if (argv[1])
	{
		put_str_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	put_str_fd("exit\n", 1);
	return (MS_BUILTIN_EXIT);
}
