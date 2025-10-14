/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:39:44 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:39:44 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

int	hd_ensure_status(char **status, t_env *env)
{
	int	value;

	if (*status)
		return (0);
	value = 0;
	if (env)
		value = env->last_status;
	*status = ft_itoa(value);
	if (!*status)
	{
		put_str_fd("minishell: malloc failed\n", 2);
		return (1);
	}
	return (0);
}

int	handle_heredoc(const t_redir_step *step, t_hd_task *task)
{
	t_hd_session	sess;
	int				result;

	if (!step->word)
		return (0);
	hd_init_session(&sess, task, step->word, step->flag);
	if (hd_open_pipe(&sess) != 0)
		return (1);
	if (hd_enable_signal(&sess) != 0)
	{
		hd_close_pipe(sess.pipefd);
		return (1);
	}
	result = hd_session_loop(&sess);
	return (hd_finalize_session(&sess, task, result));
}

int	redir_run_heredoc(const t_redir_step *step, t_redir_state *st)
{
	t_hd_task	task;

	if (hd_ensure_status(st->status, st->env) != 0)
		return (1);
	task.fd_in = st->fd_in;
	task.env = st->env;
	task.status = *(st->status);
	return (handle_heredoc(step, &task));
}
