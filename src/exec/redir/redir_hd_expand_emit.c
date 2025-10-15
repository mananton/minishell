/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_expand_emit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:38:07 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:20:50 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redir_internal.h"

int	hd_emit_single(const char *line, char *out, t_hd_emit *emit)
{
	if (emit->in_double || line[emit->i] != '\'')
		return (0);
	emit->in_single = !emit->in_single;
	out[emit->j] = line[emit->i];
	emit->j++;
	emit->i++;
	return (1);
}

int	hd_emit_double(const char *line, char *out, t_hd_emit *emit)
{
	if (emit->in_single || line[emit->i] != '"')
		return (0);
	emit->in_double = !emit->in_double;
	out[emit->j] = line[emit->i];
	emit->j++;
	emit->i++;
	return (1);
}

int	hd_emit_dollar(const char *line, char *out, t_hd_emit *emit, t_hd_ctx *ctx)
{
	size_t	step;

	if (line[emit->i] != '$' || emit->in_single)
		return (0);
	step = hd_copy_dollar(line + emit->i, out, &emit->j, ctx);
	emit->i += step;
	return (1);
}

void	hd_fill_output(const char *line, char *out, t_hd_ctx *ctx)
{
	t_hd_emit	emit;

	emit.i = 0;
	emit.j = 0;
	emit.in_single = 0;
	emit.in_double = 0;
	while (line[emit.i] && (!ctx->err || *(ctx->err) == 0))
	{
		if (hd_emit_single(line, out, &emit))
			continue ;
		if (hd_emit_double(line, out, &emit))
			continue ;
		if (hd_emit_dollar(line, out, &emit, ctx))
			continue ;
		out[emit.j] = line[emit.i];
		emit.j++;
		emit.i++;
	}
	out[emit.j] = '\0';
}
