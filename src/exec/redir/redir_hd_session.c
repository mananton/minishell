/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_session.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:33:47 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 12:12:29 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

void	hd_close_pipe(int pipefd[2])
{
	if (pipefd[0] >= 0)
		close(pipefd[0]);
	if (pipefd[1] >= 0)
		close(pipefd[1]);
	pipefd[0] = -1;
	pipefd[1] = -1;
}

void	hd_init_session(t_hd_session *sess, t_hd_task *task, const char *delim,
		int expand)
{
	sess->task = task;
	sess->delim = delim;
	sess->expand = expand;
	sess->pipefd[0] = -1;
	sess->pipefd[1] = -1;
	sess->saved_stream = NULL;
}

static int	hd_process_line(t_hd_session *sess, char *line)
{
	char	*out_line;
	char	*expanded;

	expanded = NULL;
	if (sess->expand != 0 && sess->task->env)
	{
		expanded = expand_heredoc_line(line, sess->task->env,
				sess->task->status);
		if (!expanded)
			return (1);
	}
	out_line = line;
	if (expanded)
		out_line = expanded;
	if (write_line_fd(sess->pipefd[1], out_line) != 0)
	{
		free(expanded);
		return (1);
	}
	free(expanded);
	return (0);
}

int	hd_session_loop(t_hd_session *sess)
{
	char	*line;
	int		result;

	result = 0;
	while (result == 0)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, sess->delim) == 0)
		{
			free(line);
			break ;
		}
		result = hd_process_line(sess, line);
		free(line);
	}
	return (result);
}

int	hd_finalize_session(t_hd_session *sess, t_hd_task *task, int result)
{
	hd_restore_signal(sess);
	if (g_hd_sigint == SIGINT)
	{
		hd_close_pipe(sess->pipefd);
		redir_close_fd(task->fd_in);
		return (HDOC_INTERRUPTED);
	}
	if (result != 0)
	{
		hd_close_pipe(sess->pipefd);
		redir_close_fd(task->fd_in);
		return (1);
	}
	close(sess->pipefd[1]);
	sess->pipefd[1] = -1;
	redir_assign_fd(task->fd_in, sess->pipefd[0]);
	sess->pipefd[0] = -1;
	return (0);
}
