/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:41:18 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:41:18 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

int	redir_dispatch(const t_redir_step *step, t_redir_state *st)
{
	if (step->kind == REDIR_IN)
		return (redir_open_input(step->word, st->fd_in));
	if (step->kind == REDIR_HEREDOC)
		return (redir_run_heredoc(step, st));
	return (redir_open_output(step, st->fd_out));
}

int	redir_finish(int result, int *fd_in, int *fd_out)
{
	if (result != 0 && result != HDOC_INTERRUPTED)
	{
		redir_close_fd(fd_in);
		redir_close_fd(fd_out);
	}
	if (result == HDOC_INTERRUPTED)
		redir_close_fd(fd_out);
	return (result);
}

int	open_redirs(const t_redir *r, int *fd_in, int *fd_out, t_env *env)
{
	size_t			i;
	int				result;
	char			*status;
	t_redir_state	st;

	*fd_in = -1;
	*fd_out = -1;
	if (!r || r->count == 0)
		return (0);
	status = NULL;
	st.fd_in = fd_in;
	st.fd_out = fd_out;
	st.env = env;
	st.status = &status;
	i = 0;
	result = 0;
	while (i < r->count && result == 0)
	{
		result = redir_dispatch(&r->steps[i], &st);
		i++;
	}
	free(status);
	return (redir_finish(result, fd_in, fd_out));
}
