/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_runtime.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:59:32 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 10:59:44 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>   /* open flags */
#include <unistd.h>  /* dup, dup2, close */
#include <stdio.h>   /* perror */

/* abre fds conforme r; devolve 0 se ok, 1 se erro; fd_* ficam -1 se não usados */
int	open_redirs(const t_redir *r, int *fd_in, int *fd_out)
{
	*fd_in = -1;
	*fd_out = -1;
	if (r && r->in)
	{
		*fd_in = open(r->in, O_RDONLY);
		if (*fd_in < 0)
			return (perror("minishell: open <"), 1);
	}
	if (r && r->out)
	{
		int flags = O_WRONLY | O_CREAT | (r->append ? O_APPEND : O_TRUNC);
		*fd_out = open(r->out, flags, 0644);
		if (*fd_out < 0)
			return (perror("minishell: open >"), 1);
	}
	return (0);
}

/* parent (para builtins): salva stdio, aplica dup2, e deixa fds abertos até restaurar */
int	apply_redirs_parent(const t_redir *r, int *saved_in, int *saved_out, int *fd_in, int *fd_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in < 0 || *saved_out < 0)
		return (perror("minishell: dup"), 1);
	if (open_redirs(r, fd_in, fd_out) != 0)
		return (1);
	if (*fd_in >= 0 && dup2(*fd_in, STDIN_FILENO) < 0)
		return (perror("minishell: dup2 <"), 1);
	if (*fd_out >= 0 && dup2(*fd_out, STDOUT_FILENO) < 0)
		return (perror("minishell: dup2 >"), 1);
	return (0);
}

/* desfaz redireções no parent (restaura stdio e fecha fds temporários) */
void	restore_stdio_parent(int saved_in, int saved_out, int fd_in, int fd_out)
{
	if (saved_in >= 0)
		dup2(saved_in, STDIN_FILENO);
	if (saved_out >= 0)
		dup2(saved_out, STDOUT_FILENO);
	if (saved_in >= 0)
		close(saved_in);
	if (saved_out >= 0)
		close(saved_out);
	if (fd_in >= 0)
		close(fd_in);
	if (fd_out >= 0)
		close(fd_out);
}
