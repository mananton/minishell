/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_signal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:32:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:32:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

volatile sig_atomic_t	g_hd_sigint = 0;

int	write_line_fd(int fd, const char *line)
{
	size_t	len;

	len = strlen(line);
	if (write(fd, line, len) < 0)
	{
		perror("minishell: write");
		return (1);
	}
	if (write(fd, "\n", 1) < 0)
	{
		perror("minishell: write");
		return (1);
	}
	return (0);
}

void	heredoc_sigint(int sig)
{
	g_hd_sigint = sig;
	rl_replace_line("", 0);
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
}

int	hd_open_pipe(t_hd_session *sess)
{
	if (pipe(sess->pipefd) < 0)
	{
		perror("minishell: pipe");
		return (1);
	}
	g_hd_sigint = 0;
	return (0);
}

int	hd_enable_signal(t_hd_session *sess)
{
	struct sigaction	sa;

	sess->saved_stream = rl_outstream;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint;
	if (sigaction(SIGINT, &sa, &sess->old) < 0)
	{
		rl_outstream = sess->saved_stream;
		perror("minishell: sigaction");
		return (1);
	}
	rl_outstream = stderr;
	return (0);
}

void	hd_restore_signal(t_hd_session *sess)
{
	rl_outstream = sess->saved_stream;
	sigaction(SIGINT, &sess->old, NULL);
}
