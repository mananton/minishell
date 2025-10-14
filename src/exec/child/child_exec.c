/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:23:14 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:23:14 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_builtin_child(char **argv, t_env *env)
{
	int	st;
	int	code;

	st = run_builtin(argv, env);
	if (st >= MS_BUILTIN_EXIT)
	{
		code = (st - MS_BUILTIN_EXIT) & 0xFF;
		_exit(code);
	}
	_exit(st & 0xFF);
}

static void	run_external_child(char **argv, t_env *env)
{
	char		*path;
	char *const	*envp;

	path = resolve_path(argv[0], env);
	if (!path)
	{
		put_str_fd("minishell: ", 2);
		put_str_fd(argv[0], 2);
		put_str_fd(": command not found\n", 2);
		_exit(127);
	}
	envp = NULL;
	if (env)
		envp = (char *const *)env->vars;
	execve(path, argv, envp);
	_exit(exec_error_code(path));
}

void	exec_child_command(const t_child_ctx *ctx)
{
	if (!ctx || !ctx->cmd)
		_exit(1);
	if (ctx->cmd->argv[0] && is_builtin(ctx->cmd->argv[0]))
		run_builtin_child(ctx->cmd->argv, ctx->env);
	run_external_child(ctx->cmd->argv, ctx->env);
}
