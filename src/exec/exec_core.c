/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:46:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 12:49:29 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>   /* fork, execve */
#include <sys/wait.h> /* waitpid, macros */
#include <stdlib.h>   /* free */
#include <stdio.h>    /* perror */

/* mensagem padrão “command not found” */
static int	cmd_not_found(const char *cmd)
{
	put_str_fd("minishell: command not found: ", 2);
	if (cmd)
		put_str_fd(cmd, 2);
	put_str_fd("\n", 2);
	return (127);
}

/* filho: tenta execve; em erro, perror e exit(126) */
static void	child_exec_or_die(char *path, char **argv, char *const *envp)
{
	execve(path, argv, envp);
	perror("minishell: execve");
	free(path);
	exit(126);
}

/* pai: espera e normaliza status */
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

/* API pública: executa comando externo (sem redir/pipes) */
int	exec_external(char **argv, t_env *env)
{
	pid_t			pid;
	char			*path;
	char *const		*envp;

	if (!argv || !argv[0])
		return (0);
	path = resolve_path(argv[0], env);
	if (!path)
		return (cmd_not_found(argv[0]));
	envp = NULL;
	if (env)
		envp = (char *const *)env->vars;
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		free(path);
		return (1);
	}
	if (pid == 0)
		child_exec_or_die(path, argv, envp);
	free(path);
	return (wait_and_status(pid));
}