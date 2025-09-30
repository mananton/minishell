/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:49:38 by mananton          #+#    #+#             */
/*   Updated: 2025/09/30 11:49:39 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <stdint.h>

/* ajusta errno manualmente */
void	set_error_code(int code)
{
	errno = code;
}

/* conversão robusta string → long long */
long long	str_to_ll(const char *s)
{
	int					sign;
	unsigned long long	acc;
	long long			result;

	acc = 0;
	if (!s)
		return (set_error_code(EINVAL), 0);
	sign = (s[0] != '-') - (s[0] == '-');
	if (s[0] == '-' || s[0] == '+')
		s++;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (set_error_code(EINVAL), 0);
		else if (acc > (acc * 10) + (*s - '0'))
			return (set_error_code(ERANGE), 0);
		else
			acc = (acc * 10) + (*s - '0');
		s++;
	}
	result = acc * sign;
	if (result < INT64_MIN || result > INT64_MAX)
		return (set_error_code(ERANGE), 0);
	return (result);
}

/* valida e retorna número long */
long	validate_number(char *text)
{
	long long	val;

	if (!text || !*text)
		return (set_error_code(EINVAL), 0);
	errno = 0;
	val = str_to_ll(text);
	if (errno)
		return (set_error_code(EINVAL), 0);
	return (val);
}

/* normaliza para código de status (0–255) */
int	normalize_status(char *text)
{
	if (!text)
		return (0);
	return (validate_number(text) & 255);
}

/*
 * builtin exit:
 * - sem args → sai com 0
 * - exit N válido → sai com N % 256
 * - exit N inválido → sai com 2
 * - exit com >1 arg → erro, mas não sai
 */
int	builtin_exit(t_env *env, char **argv)
{
	int	code;

	(void)env;
	errno = 0;
	if (!argv[1])
	{
		put_str_fd("exit\n", 1);
		return (MS_BUILTIN_EXIT);
	}
	code = normalize_status(argv[1]);
	if (errno)
	{
		put_str_fd("exit\n", 1);
		put_str_fd("minishell: exit: ", 2);
		put_str_fd(argv[1], 2);
		put_str_fd(": numeric argument required\n", 2);
		return (MS_BUILTIN_EXIT + 2);
	}
	if (argv[2])
	{
		put_str_fd("minishell: exit: too many arguments\n", 2);
		return (MS_BUILTIN_EXIT + 1);
	}
	put_str_fd("exit\n", 1);
	return (MS_BUILTIN_EXIT + code);
}
