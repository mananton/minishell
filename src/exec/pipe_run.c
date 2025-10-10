/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:32:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 14:32:44 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/exec/pipe_run.c */
#include "minishell.h"
#include <unistd.h>    /* pipe, fork, close, _exit */
#include <sys/wait.h>  /* waitpid */
#include <stdlib.h>    /* malloc, free */
#include <stdio.h>     /* perror */

/* forward: implementada noutro ficheiro */
int	run_child_cmd(int idx, int n, int fds[][2], t_cmd *cmd, t_env *env);

/* cria N-1 pipes (fds[i][0]=read, fds[i][1]=write) */
static int	make_pipes(int n, int fds[][2])
{
	int i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(fds[i]) < 0)
			return (perror("minishell: pipe"), 1);
		i++;
	}
	return (0);
}

/* fecha todos os pipes no PAI */
static void	close_pipes_parent(int n, int fds[][2])
{
	int i;

	i = 0;
	while (i < n - 1)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

/* espera N pids; devolve status do último comando */
static int	wait_all_pids(pid_t *p, int n)
{
	int i;
	int s;
	int last;

	i = 0;
	last = 0;
	while (i < n)
	{
		if (waitpid(p[i], &s, 0) > 0 && i == n - 1)
		{
			if (WIFEXITED(s))
				last = WEXITSTATUS(s);
			else if (WIFSIGNALED(s))
				last = 128 + WTERMSIG(s);
			else
				last = 1;
		}
		i++;
	}
	return (last);
}

/* aloca arrays: pids (sempre) e fds (se n>1). devolve 0/erro. */
static int	alloc_arrays(int n, pid_t **pids_out, int (**fds_out)[2])
{
	pid_t	*pids;

	*pids_out = NULL;
	*fds_out = NULL;
	pids = (pid_t *)malloc(sizeof(pid_t) * n);
	if (!pids)
		return (1);
	*pids_out = pids;
	if (n > 1)
	{
		*fds_out = (int (*)[2])malloc(sizeof(int[2]) * (n - 1));
		if (!*fds_out)
			return (free(pids), *pids_out = NULL, 1);
	}
	return (0);
}

/* faz fork para todos os comandos do pipeline; lança run_child_cmd nos filhos */
static int	fork_all_children(int n, int fds[][2], t_cmd *cmds, t_env *env,
							pid_t *pids)
{
	int		i;
	pid_t	p;

	i = 0;
	while (i < n)
	{
		p = fork();
		if (p < 0)
			return (perror("minishell: fork"), 1);
		if (p == 0)
			run_child_cmd(i, n, fds, &cmds[i], env);
		pids[i] = p;
		i++;
	}
	return (0);
}

/* no pai: fecha pipes (se houver), espera filhos e liberta memória */
static int	cleanup_and_wait(int n, int fds[][2], pid_t *pids)
{
	int status;

	if (n > 1)
		close_pipes_parent(n, fds);
	status = wait_all_pids(pids, n);
	free(pids);
	if (n > 1)
		free(fds);
	return (status);
}

/* API: executa o pipeline (N comandos) e devolve status do último */
int	exec_pipeline(t_cmd *cmds, int n, t_env *env)
{
	int			status;
	int			(*fds)[2];
	pid_t		*pids;

	if (n <= 0)
		return (0);
	if (alloc_arrays(n, &pids, &fds) != 0)
		return (1);
	if (n > 1)
	{
		if (make_pipes(n, fds) != 0)
			return (free(pids), free(fds), 1);
	}
	if (fork_all_children(n, fds, cmds, env, pids) != 0)
	{
		if (n > 1)
			close_pipes_parent(n, fds);
		free(pids);
		if (n > 1)
			free(fds);
		return (1);
	}
	status = cleanup_and_wait(n, fds, pids);
	return (status);
}
