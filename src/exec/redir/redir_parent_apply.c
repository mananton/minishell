/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parent_apply.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:42:56 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 12:12:40 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

int	apply_redirs_parent(const t_redir *r, t_parent_redir *io, t_env *env)
{
	int	result;

	if (save_stdio_parent(&io->saved_in, &io->saved_out) != 0)
		return (1);
	io->fd_in = -1;
	io->fd_out = -1;
	result = open_redirs(r, &io->fd_in, &io->fd_out, env);
	if (result != 0)
	{
		redir_close_saved(io);
		return (result);
	}
	if (install_redirs_parent(io->saved_in, io->saved_out, &io->fd_in,
			&io->fd_out) != 0)
	{
		io->fd_in = -1;
		io->fd_out = -1;
		return (1);
	}
	return (0);
}

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
