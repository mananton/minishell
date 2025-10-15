/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:10:31 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:57:32 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell_internal.h"

static int	prepare_pipeline(char **argv, char ****segs, int *count,
		t_cmd **cmds)
{
	if (split_pipeline(argv, segs, count) != 0)
	{
		free_segments(segs, *count);
		return (SHELL_PIPE_NONE);
	}
	if (*count <= 1)
	{
		free_segments(segs, *count);
		return (SHELL_PIPE_NONE);
	}
	if (build_cmdlist(*segs, *count, cmds) != 0)
	{
		put_str_fd("minishell: syntax error near '|'\n", 2);
		free_segments(segs, *count);
		return (SHELL_PIPE_ERROR);
	}
	return (SHELL_PIPE_EXEC);
}

int	shell_try_pipeline(t_env *env, t_shell_cmd *cmd)
{
	char	***segs;
	int		nseg;
	t_cmd	*cmds;
	int		state;
	int		status;

	segs = NULL;
	nseg = 0;
	state = prepare_pipeline(cmd->argv, &segs, &nseg, &cmds);
	if (state != SHELL_PIPE_EXEC)
		return (state);
	status = exec_pipeline(cmds, nseg, env);
	env->last_status = status & 0xFF;
	free_cmdlist(cmds, nseg);
	free_segments(&segs, nseg);
	return (SHELL_PIPE_EXEC);
}
