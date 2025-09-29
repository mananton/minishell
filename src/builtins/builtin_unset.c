/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:16:57 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 13:23:08 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* retorna 1 se a string contém '=' (que não aceitamos aqui) */
static int	has_equal(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

/*
** builtin_unset (mínimo):
** - aceita: unset KEY [KEY...]
** - rejeita: argumentos com '=' → erro e continua nos próximos
** - não falha se a KEY não existir
*/
int	builtin_unset(t_env *env, char **argv)
{
	size_t	i;
	int		status;

	if (!env || !argv)
		return (1);
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (!*argv[i] || has_equal(argv[i]) || !is_valid_ident(argv[i]))
		{
			put_str_fd("minishell: unset: invalid name: ", 2);
			put_str_fd(argv[i], 2);
			put_str_fd("\n", 2);
			status = 1;
		}
		else
			(void)env_unset(env, argv[i]);
		i++;
	}
	return (status);
}
