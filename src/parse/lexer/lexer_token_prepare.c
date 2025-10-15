/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_prepare.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:05:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:00:48 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

int	token_build_prepare(t_token_build *build,
		const t_expand_ctx *base, size_t start, int *quoted_any)
{
	t_len_runner	runner;
	size_t			length;
	int				length_err;

	length_err = 0;
	len_runner_init(&runner, base, start);
	runner.err = &length_err;
	runner.quoted_any = quoted_any;
	length = len_runner_execute(&runner);
	if (length_err != 0)
		return (1);
	build->emit.out = (char *)malloc(length + 1);
	if (!build->emit.out)
		return (1);
	build->ctx = *base;
	init_copy_state(&build->copy, start);
	build->out_pos = 0;
	build->emit.pos = &build->out_pos;
	return (0);
}

int	build_token_body(t_expand_ctx *ctx, t_copy_state *state,
		t_emit *emit)
{
	while (state->index < ctx->end)
	{
		if (handle_quote_char(ctx->str[state->index], &state->quotes, NULL))
		{
			state->index++;
			continue ;
		}
		if (!state->quotes.in_single && ctx->str[state->index] == '\\')
		{
			copy_escape(ctx, state, emit);
			continue ;
		}
		if (ctx->str[state->index] == '$')
		{
			if (process_dollar_copy(ctx, &state->index, emit,
					state->quotes.in_single) != 0)
				return (1);
			continue ;
		}
		emit->out[*emit->pos] = ctx->str[state->index];
		*emit->pos += 1;
		state->index += 1;
	}
	emit->out[*emit->pos] = '\0';
	return (0);
}
