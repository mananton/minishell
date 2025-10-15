/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:06:55 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:57:11 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_internal.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

int	shell_read_line(t_env *env, t_shell_cmd *cmd)
{
	cmd->line = readline("minishell$ ");
	signals_update_env(env);
	if (!cmd->line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (SHELL_READ_EOF);
	}
	if (*cmd->line)
		add_history(cmd->line);
	return (SHELL_READ_OK);
}

int	shell_prepare_args(t_env *env, t_shell_cmd *cmd)
{
	cmd->argv = split_args_quotes(cmd->line, env);
	if (!cmd->argv)
		return (SHELL_STEP_SKIP);
	if (expand_wildcards(&cmd->argv) != 0)
	{
		put_str_fd("minishell: malloc failed\n", 2);
		free_argv(cmd->argv);
		cmd->argv = NULL;
		return (SHELL_STEP_SKIP);
	}
	return (SHELL_STEP_OK);
}
