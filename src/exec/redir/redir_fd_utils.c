/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:40:26 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:40:26 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

void	redir_close_fd(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void	redir_assign_fd(int *fd, int new_fd)
{
	redir_close_fd(fd);
	*fd = new_fd;
}

int	redir_open_input(const char *path, int *fd_in)
{
	int	fd;

	redir_close_fd(fd_in);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell: open <");
		return (1);
	}
	*fd_in = fd;
	return (0);
}

int	redir_open_output(const t_redir_step *step, int *fd_out)
{
	int	flags;
	int	fd;

	redir_close_fd(fd_out);
	flags = O_WRONLY | O_CREAT;
	if (step->kind == REDIR_OUT_TRUNC)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	fd = open(step->word, flags, 0644);
	if (fd < 0)
	{
		perror("minishell: open >");
		return (1);
	}
	*fd_out = fd;
	return (0);
}
