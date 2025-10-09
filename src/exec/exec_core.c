/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:46:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 11:58:18 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>    /* fork, execve, dup, dup2, close, _exit */
#include <sys/wait.h>  /* waitpid, WIFEXITED... */
#include <stdlib.h>    /* free */
#include <stdio.h>     /* perror */

/* devolve envp a partir do nosso t_env */
static char *const	*envp_from_env(t_env *env)
{
	if (env)
		return ((char *const *)env->vars);  // vetor KEY=VALUE
	return (NULL);                           // sem env → sem vars
}

/* dup2 no filho (se fd válido) + fecha */
static int	child_apply_dup(int fd, int stdio, const char *who)
{
	if (fd >= 0)                             // só se abriu ficheiro
	{
		if (dup2(fd, stdio) < 0)            // substitui stdin/stdout
			return (perror(who), 1);        // erro → reporta
		close(fd);                           // fecha o fd original
	}
	return (0);                              // OK
}

/* espera o filho e normaliza status */
static int	wait_and_status(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);                // espera término
	if (WIFEXITED(status))                   // saiu por exit(...)
		return (WEXITSTATUS(status));        // 0..255
	if (WIFSIGNALED(status))                 // terminou por sinal
		return (128 + WTERMSIG(status));     // convenção POSIX
	return (1);                              // fallback
}

/* cria filho, aplica redireções no filho e faz execve */
static int	spawn_and_exec(char *path, char **argv, t_env *env, const t_redir *r)
{
	pid_t		pid;
	int			fd_in;
	int			fd_out;
	char *const	*envp;

	envp = envp_from_env(env);               // prepara envp
	pid = fork();                            // cria processo filho
	if (pid < 0)
		return (perror("minishell: fork"), 1);
	if (pid == 0)                            // filho
	{
		if (open_redirs(r, &fd_in, &fd_out) != 0)
			_exit(1);                        // erro ao abrir
		if (child_apply_dup(fd_in, STDIN_FILENO, "minishell: dup2 <") != 0)
			_exit(1);
		if (child_apply_dup(fd_out, STDOUT_FILENO, "minishell: dup2 >") != 0)
			_exit(1);
		execve(path, argv, envp);            // tenta executar
		_exit(exec_error_code(path));        // classifica falha do execve
	}
	return (wait_and_status(pid));           // pai: devolve status
}

/* público: exec externo com redireções */
int	exec_external_redir(char **argv, t_env *env, const t_redir *r)
{
	char	*path;
	int		rc;

	if (!argv || !argv[0])                   // linha vazia
		return (0);
	path = resolve_path(argv[0], env);       // PATH ou caminho com '/'
	if (!path)                               // não encontrado no PATH
	{
		put_str_fd("minishell: command not found: ", 2);
		put_str_fd(argv[0], 2);
		put_str_fd("\n", 2);
		return (127);
	}
	rc = spawn_and_exec(path, argv, env, r); // cria filho e executa
	free(path);                               // liberta caminho
	return (rc);                              // status normalizado
}
