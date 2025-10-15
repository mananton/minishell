/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:05:44 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:34:34 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_INTERNAL_H
# define SHELL_INTERNAL_H

# include "shell/loop.h"
# include "redir/redir.h"
# include "parse/lexer.h"
# include "parse/pipeline.h"
# include "exec/exec.h"

typedef struct s_shell_cmd
{
	char	*line;
	char	**argv;
	char	**argv_clean;
	t_redir	redir;
}	t_shell_cmd;

# define SHELL_READ_OK 0
# define SHELL_READ_EOF 1

# define SHELL_STEP_OK 0
# define SHELL_STEP_SKIP 1

# define SHELL_PIPE_NONE 0
# define SHELL_PIPE_EXEC 1
# define SHELL_PIPE_ERROR 2

void	shell_cmd_init(t_shell_cmd *cmd);
void	shell_cmd_cleanup(t_shell_cmd *cmd);

int		shell_read_line(t_env *env, t_shell_cmd *cmd);
int		shell_prepare_args(t_env *env, t_shell_cmd *cmd);
int		shell_try_pipeline(t_env *env, t_shell_cmd *cmd);
int		shell_parse_redirs_stage(t_shell_cmd *cmd);
int		shell_execute_stage(t_env *env, t_shell_cmd *cmd,
			int *exit_flag, int *exit_code);

#endif
