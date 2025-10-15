/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:46:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:12:01 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_core_internal.h"

static char *const	*envp_from_env(t_env *env)
{
	if (env)
		return ((char *const *)env->vars);
	return (NULL);
}

static int	child_apply_dup(int fd, int stdio, const char *who)
{
	if (fd >= 0)
	{
		if (dup2(fd, stdio) < 0)
		{
			perror(who);
			return (1);
		}
		close(fd);
	}
	return (0);
}

static int	wait_and_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	child_exec(char *path, char **argv, t_env *env, const t_redir *r)
{
	char *const	*envp;
	int			fd_in;
	int			fd_out;
	int			res;

	envp = envp_from_env(env);
	signals_setup_child();
	res = open_redirs(r, &fd_in, &fd_out, env);
	if (res != 0)
	{
		if (res == HDOC_INTERRUPTED)
			exit(HDOC_INTERRUPTED);
		exit(1);
	}
	if (child_apply_dup(fd_in, STDIN_FILENO, "minishell: dup2 <") != 0)
		exit(1);
	if (child_apply_dup(fd_out, STDOUT_FILENO, "minishell: dup2 >") != 0)
		exit(1);
	execve(path, argv, envp);
	exit(exec_error_code(path));
}

int	spawn_and_exec(char *path, char **argv, t_env *env, const t_redir *r)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
		child_exec(path, argv, env, r);
	return (wait_and_status(pid));
}
