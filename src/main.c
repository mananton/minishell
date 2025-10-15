/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:47:01 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 11:58:22 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "shell/loop.h"

static int	init_minishell(t_env **env, char **envp)
{
	*env = env_init(envp);
	if (!*env)
	{
		put_str_fd("minishell: env init failed\n", 2);
		return (1);
	}
	(void)env_fix_shlvl(*env);
	signals_setup_interactive();
	rl_catch_signals = 0;
	return (0);
}

int	main(int argc, char **argv_unused, char **envp)
{
	t_env	*env;
	int		exit_code;

	(void)argc;
	(void)argv_unused;
	if (init_minishell(&env, envp) != 0)
		return (1);
	exit_code = minishell_run(env);
	env_free(env);
	return (exit_code);
}
