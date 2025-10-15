/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:12:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:12:00 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_loop_internal.h"

static int	handle_pipeline_stage(t_env *env, t_shell_cmd *cmd)
{
	int	status;

	status = shell_try_pipeline(env, cmd);
	if (status == SHELL_PIPE_NONE)
		return (SHELL_STEP_OK);
	shell_cmd_cleanup(cmd);
	return (SHELL_STEP_SKIP);
}

static int	prepare_command(t_env *env, t_shell_cmd *cmd)
{
	if (shell_prepare_args(env, cmd) != SHELL_STEP_OK)
	{
		shell_cmd_cleanup(cmd);
		return (SHELL_STEP_SKIP);
	}
	if (handle_pipeline_stage(env, cmd) != SHELL_STEP_OK)
		return (SHELL_STEP_SKIP);
	if (shell_parse_redirs_stage(cmd) != SHELL_STEP_OK)
	{
		shell_cmd_cleanup(cmd);
		return (SHELL_STEP_SKIP);
	}
	return (SHELL_STEP_OK);
}

int	minishell_run(t_env *env)
{
	t_shell_cmd	cmd;
	int			exit_flag;
	int			exit_code;

	shell_cmd_init(&cmd);
	exit_flag = 0;
	exit_code = 0;
	while (!exit_flag)
	{
		if (shell_read_line(env, &cmd) == SHELL_READ_EOF)
			break ;
		if (prepare_command(env, &cmd) != SHELL_STEP_OK)
			continue ;
		shell_execute_stage(env, &cmd, &exit_flag, &exit_code);
		shell_cmd_cleanup(&cmd);
	}
	shell_cmd_cleanup(&cmd);
	if (exit_flag)
		return (exit_code & 0xFF);
	return (0);
}
