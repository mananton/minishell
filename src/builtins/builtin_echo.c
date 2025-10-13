/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 10:12:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:07:20 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* verifica se arg é do tipo -n, -nnn, etc. */
static int	is_flag_n(const char *s)
{
	size_t	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	if (s[i] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	builtin_echo(t_env *env, char **argv)
{
	int	newline;
	int	i;

	(void)env;
	newline = 1;
	i = 1;
	while (argv[i] && is_flag_n(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		put_str_fd(argv[i], 1);
		if (argv[i + 1])
			put_str_fd(" ", 1);
		i++;
	}
	if (newline)
		put_str_fd("\n", 1);
	return (0);
}
