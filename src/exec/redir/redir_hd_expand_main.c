/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_expand_main.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:38:53 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:38:53 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

char	*expand_heredoc_line(const char *line, t_env *env, const char *status)
{
	t_hd_ctx	ctx;
	size_t		total;
	char		*out;
	int			err;

	err = 0;
	ctx.env = env;
	ctx.status = status;
	ctx.err = &err;
	total = hd_total_len(line, &ctx);
	if (err != 0)
		return (NULL);
	out = (char *)malloc(total + 1);
	if (!out)
		return (NULL);
	err = 0;
	ctx.err = &err;
	hd_fill_output(line, out, &ctx);
	if (err != 0)
	{
		free(out);
		return (NULL);
	}
	return (out);
}
