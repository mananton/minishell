/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parent.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:42:07 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:21:29 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redir_internal.h"

int	save_stdio_parent(int *saved_in, int *saved_out)
{
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in < 0 || *saved_out < 0)
	{
		perror("minishell: dup");
		if (*saved_in >= 0)
			close(*saved_in);
		if (*saved_out >= 0)
			close(*saved_out);
		return (1);
	}
	return (0);
}

static int	install_in_parent(int saved_in, int saved_out, int *fd_in,
		int *fd_out)
{
	if (*fd_in >= 0)
	{
		if (dup2(*fd_in, STDIN_FILENO) < 0)
		{
			perror("minishell: dup2 <");
			if (*fd_in >= 0)
				close(*fd_in);
			if (*fd_out >= 0)
				close(*fd_out);
			close(saved_in);
			close(saved_out);
			return (1);
		}
		close(*fd_in);
		*fd_in = -1;
	}
	return (0);
}

static int	install_out_parent(int saved_in, int saved_out, int *fd_in,
		int *fd_out)
{
	if (*fd_out >= 0)
	{
		if (dup2(*fd_out, STDOUT_FILENO) < 0)
		{
			perror("minishell: dup2 >");
			if (*fd_in >= 0)
				close(*fd_in);
			if (*fd_out >= 0)
				close(*fd_out);
			close(saved_in);
			close(saved_out);
			return (1);
		}
		close(*fd_out);
		*fd_out = -1;
	}
	return (0);
}

int	install_redirs_parent(int saved_in, int saved_out, int *fd_in, int *fd_out)
{
	if (install_in_parent(saved_in, saved_out, fd_in, fd_out) != 0)
		return (1);
	if (install_out_parent(saved_in, saved_out, fd_in, fd_out) != 0)
		return (1);
	return (0);
}

void	redir_close_saved(t_parent_redir *io)
{
	if (io->saved_in >= 0)
	{
		close(io->saved_in);
		io->saved_in = -1;
	}
	if (io->saved_out >= 0)
	{
		close(io->saved_out);
		io->saved_out = -1;
	}
}
