/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_context.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:06:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:56:49 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_internal.h"

void	shell_cmd_init(t_shell_cmd *cmd)
{
	cmd->line = NULL;
	cmd->argv = NULL;
	cmd->argv_clean = NULL;
	cmd->redir.steps = NULL;
	cmd->redir.count = 0;
}

void	shell_cmd_cleanup(t_shell_cmd *cmd)
{
	if (cmd->line)
	{
		free(cmd->line);
		cmd->line = NULL;
	}
	if (cmd->argv)
	{
		free_argv(cmd->argv);
		cmd->argv = NULL;
	}
	if (cmd->argv_clean)
	{
		free_argv(cmd->argv_clean);
		cmd->argv_clean = NULL;
	}
	redir_clear(&cmd->redir);
	cmd->redir.steps = NULL;
	cmd->redir.count = 0;
}
