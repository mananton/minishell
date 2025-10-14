/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:48:37 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 15:48:37 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

static int	copy_escape_double(const t_expand_ctx *ctx, t_copy_state *state,
		t_emit *emit)
{
	const char	*str;

	str = ctx->str;
	if (state->quotes.in_double)
	{
		if (str[state->index + 1] && is_dquote_escapable(str[state->index + 1]))
		{
			emit->out[*emit->pos] = str[state->index + 1];
			*emit->pos += 1;
			state->index += 2;
			return (1);
		}
		emit->out[*emit->pos] = str[state->index];
		*emit->pos += 1;
		state->index += 1;
		return (1);
	}
	return (0);
}

void	copy_escape(const t_expand_ctx *ctx, t_copy_state *state,
		t_emit *emit)
{
	const char	*str;

	str = ctx->str;
	if (copy_escape_double(ctx, state, emit))
		return ;
	if (str[state->index + 1])
	{
		if (str[state->index + 1] == '*' || str[state->index + 1] == '?'
			|| str[state->index + 1] == '[')
			state->inhibit_glob = 1;
		emit->out[*emit->pos] = str[state->index + 1];
		*emit->pos += 1;
		state->index += 2;
	}
	else
		state->index += 1;
}

void	init_copy_state(t_copy_state *state, size_t start)
{
	state->index = start;
	state->quotes.in_single = 0;
	state->quotes.in_double = 0;
	state->inhibit_glob = 0;
}

unsigned int	token_flags(int quoted_any, int inhibit_glob)
{
	unsigned int	flags;

	flags = 0;
	if (quoted_any)
		flags |= TOKEN_META_QUOTED;
	if (inhibit_glob)
		flags |= TOKEN_META_NO_GLOB;
	return (flags);
}
