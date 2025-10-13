/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:58:35 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 13:56:54 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (strcmp(cmd, "pwd") == 0)
		return (1);
	if (strcmp(cmd, "echo") == 0)
		return (1);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	if (strcmp(cmd, "export") == 0)
		return (1);
	if (strcmp(cmd, "unset") == 0)
		return (1);
	if (strcmp(cmd, "exit") == 0)
		return (1);
	if (strcmp(cmd, "status") == 0)
		return (1);
	return (0);
}

int	run_builtin(char **argv, t_env *env)
{
	if (!argv || !argv[0])
		return (1);
	if (strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd(env));
	if (strcmp(argv[0], "echo") == 0)
		return (builtin_echo(env, argv));
	if (strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, env));
	if (strcmp(argv[0], "env") == 0)
		return (builtin_env(env, argv));
	if (strcmp(argv[0], "export") == 0)
		return (builtin_export(env, argv));
	if (strcmp(argv[0], "unset") == 0)
		return (builtin_unset(env, argv));
	if (strcmp(argv[0], "exit") == 0)
		return (builtin_exit(env, argv));
	if (strcmp(argv[0], "status") == 0)
		return (builtin_status(env, argv));
	return (127);
}
