/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core_external.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:46:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:37:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_core_internal.h"

int	exec_external_redir(char **argv, t_env *env, const t_redir *r)
{
	char	*path;
	int		status;

	if (!argv || !argv[0])
		return (0);
	path = resolve_path(argv[0], env);
	if (!path)
	{
		put_str_fd("minishell: ", 2);
		put_str_fd(argv[0], 2);
		put_str_fd(": command not found\n", 2);
		return (127);
	}
	status = spawn_and_exec(path, argv, env, r);
	free(path);
	return (status);
}
