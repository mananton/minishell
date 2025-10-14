/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:44:03 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 10:42:10 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	err3(const char *path, const char *msg)
{
	put_str_fd("minishell: ", 2);
	if (path)
		put_str_fd(path, 2);
	else
		put_str_fd("(null)", 2);
	put_str_fd(": ", 2);
	put_str_fd(msg, 2);
	put_str_fd("\n", 2);
}

static int	handle_stat_error(const char *path)
{
	if (errno == ENOENT)
	{
		err3(path, "No such file or directory");
		return (127);
	}
	if (errno == EACCES)
	{
		err3(path, "Permission denied");
		return (126);
	}
	err3(path, "Error");
	return (126);
}

int	exec_error_code(const char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (handle_stat_error(path));
	if (S_ISDIR(st.st_mode))
	{
		err3(path, "Is a directory");
		return (126);
	}
	if (access(path, X_OK) != 0)
	{
		if (errno == EACCES)
			err3(path, "Permission denied");
		else
			err3(path, "Not executable");
		return (126);
	}
	err3(path, "execve failed");
	return (126);
}
