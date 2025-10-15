/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:11:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 12:23:56 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_loop_internal.h"

int	shell_parse_redirs_stage(t_shell_cmd *cmd)
{
	if (parse_redirs(cmd->argv, &cmd->argv_clean, &cmd->redir) != 0)
		return (SHELL_STEP_SKIP);
	return (SHELL_STEP_OK);
}

static int	exec_builtin_redir(t_env *env, t_shell_cmd *cmd,
		t_parent_redir *io)
{
	int	status;
	int	rc;

	*io = (t_parent_redir){-1, -1, -1, -1};
	rc = apply_redirs_parent(&cmd->redir, io, env);
	if (rc == 0)
	{
		status = run_builtin(cmd->argv_clean, env);
		restore_stdio_parent(io->saved_in, io->saved_out,
			io->fd_in, io->fd_out);
		return (status);
	}
	if (rc == HDOC_INTERRUPTED)
		return (HDOC_INTERRUPTED);
	return (1);
}

static void	run_builtin_with_redir(t_env *env, t_shell_cmd *cmd,
		int *exit_flag, int *exit_code)
{
	t_parent_redir	io;
	int				status;

	status = exec_builtin_redir(env, cmd, &io);
	if (status >= MS_BUILTIN_EXIT)
	{
		*exit_flag = 1;
		*exit_code = status - MS_BUILTIN_EXIT;
		return ;
	}
	env->last_status = status & 0xFF;
}

static void	run_external_with_redir(t_env *env, t_shell_cmd *cmd)
{
	int	status;

	status = exec_external_redir(cmd->argv_clean, env, &cmd->redir);
	env->last_status = status & 0xFF;
}

int	shell_execute_stage(t_env *env, t_shell_cmd *cmd,
		int *exit_flag, int *exit_code)
{
	if (!cmd->argv_clean || !cmd->argv_clean[0])
		return (SHELL_STEP_OK);
	if (is_builtin(cmd->argv_clean[0]))
	{
		run_builtin_with_redir(env, cmd, exit_flag, exit_code);
		return (SHELL_STEP_OK);
	}
	if (*cmd->argv_clean[0])
		run_external_with_redir(env, cmd);
	return (SHELL_STEP_OK);
}
