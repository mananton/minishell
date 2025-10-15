/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_len.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:47:59 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:00:56 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

void	len_escape(const t_expand_ctx *ctx, t_len_state *state)
{
	const char	*str;

	str = ctx->str;
	if (state->quotes.in_double)
	{
		if (str[state->index + 1] && is_dquote_escapable(str[state->index + 1]))
		{
			state->count++;
			state->index += 2;
			return ;
		}
		state->count++;
		state->index += 1;
		return ;
	}
	if (str[state->index + 1])
	{
		state->count++;
		state->index += 2;
	}
	else
		state->index += 1;
}

void	len_runner_init(t_len_runner *runner, const t_expand_ctx *base,
		size_t start)
{
	runner->ctx = *base;
	runner->state.index = start;
	runner->state.count = 0;
	runner->state.quotes.in_single = 0;
	runner->state.quotes.in_double = 0;
}

static int	len_handle_special(t_len_runner *runner, char current)
{
	if (!runner->state.quotes.in_single && current == '\\')
	{
		len_escape(&runner->ctx, &runner->state);
		return (1);
	}
	if (current == '$')
	{
		runner->state.count += process_dollar_len(&runner->ctx,
				&runner->state.index, runner->state.quotes.in_single,
				runner->err);
		return (1);
	}
	return (0);
}

void	len_process_char(t_len_runner *runner)
{
	char	current;

	current = runner->ctx.str[runner->state.index];
	if (handle_quote_char(current, &runner->state.quotes,
			runner->quoted_any))
	{
		runner->state.index++;
		return ;
	}
	if (len_handle_special(runner, current))
		return ;
	runner->state.count++;
	runner->state.index++;
}

size_t	len_runner_execute(t_len_runner *runner)
{
	while (runner->state.index < runner->ctx.end && *(runner->err) == 0)
		len_process_char(runner);
	return (runner->state.count);
}
