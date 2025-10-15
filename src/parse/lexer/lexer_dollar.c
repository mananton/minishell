/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:47:18 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:01:02 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

size_t	dollar_literal_len(size_t *i)
{
	(*i)++;
	return (1);
}

int	dollar_literal_copy(const char *s, size_t *i, t_emit *emit)
{
	emit->out[*emit->pos] = s[*i];
	*emit->pos += 1;
	*i += 1;
	return (0);
}

size_t	process_dollar_len(t_expand_ctx *ctx, size_t *index,
		int in_single, int *err)
{
	size_t	name_len;
	size_t	start;

	if (in_single)
		return (dollar_literal_len(index));
	if (*index + 1 >= ctx->end)
		return (dollar_literal_len(index));
	if (ctx->str[*index + 1] == '?')
	{
		*index += 2;
		return (ctx->status_len);
	}
	if (!is_var_start(ctx->str[*index + 1]))
		return (dollar_literal_len(index));
	name_len = var_name_len(ctx->str, *index + 1, ctx->end);
	start = *index + 1;
	*index += name_len + 1;
	return (var_value_len(ctx->env, ctx->str + start, name_len, err));
}

int	process_dollar_copy(t_expand_ctx *ctx, size_t *index,
		t_emit *emit, int in_single)
{
	size_t	name_len;
	size_t	start;

	if (in_single)
		return (dollar_literal_copy(ctx->str, index, emit));
	if (*index + 1 >= ctx->end)
		return (dollar_literal_copy(ctx->str, index, emit));
	if (ctx->str[*index + 1] == '?')
	{
		copy_status(emit->out, emit->pos, ctx->status);
		*index += 2;
		return (0);
	}
	if (!is_var_start(ctx->str[*index + 1]))
		return (dollar_literal_copy(ctx->str, index, emit));
	name_len = var_name_len(ctx->str, *index + 1, ctx->end);
	start = *index + 1;
	if (copy_var_value(emit, ctx->env, ctx->str + start, name_len) != 0)
		return (1);
	*index += name_len + 1;
	return (0);
}
