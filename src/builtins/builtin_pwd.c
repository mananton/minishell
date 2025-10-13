/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:56:43 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 13:56:37 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_env *env)
{
	char		*cwd;
	const char	*fallback;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		put_str_fd(cwd, 1);
		put_str_fd("\n", 1);
		free(cwd);
		return (0);
	}
	perror("minishell: pwd: getcwd");
	fallback = env_get(env, "PWD");
	if (fallback)
	{
		put_str_fd(fallback, 1);
		put_str_fd("\n", 1);
	}
	return (1);
}
