/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build_tokens.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:50:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:58:50 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

void	init_expand_ctx(t_expand_ctx *expand, const t_build_ctx *ctx,
		size_t end)
{
	expand->str = ctx->line;
	expand->end = end;
	expand->env = ctx->env;
	expand->status = ctx->status;
	expand->status_len = ctx->status_len;
}

int	mark_token_error(t_build_ctx *ctx)
{
	ctx->argv[ctx->arg_index] = NULL;
	return (1);
}

int	append_token(t_build_ctx *ctx)
{
	t_expand_ctx	expand;
	int				quoted_any;
	size_t			end;
	size_t			len;

	len = token_len(ctx->line, ctx->index, &end);
	if (len == (size_t)-1)
		return (mark_token_error(ctx));
	init_expand_ctx(&expand, ctx, end);
	quoted_any = 0;
	ctx->argv[ctx->arg_index] = build_token(&expand, ctx->index,
			&quoted_any);
	if (!ctx->argv[ctx->arg_index])
		return (mark_token_error(ctx));
	ctx->arg_index++;
	ctx->index = skip_blanks(ctx->line, end);
	return (0);
}

int	build_tokens(t_build_ctx *ctx)
{
	ctx->index = skip_blanks(ctx->line, 0);
	while (ctx->line[ctx->index])
	{
		if (append_token(ctx) != 0)
			return (1);
	}
	ctx->argv[ctx->arg_index] = NULL;
	return (0);
}

char	**split_build_argv(const char *line, size_t count, t_env *env)
{
	t_build_ctx	ctx;

	if (init_build_ctx(&ctx, line, count, env) != 0)
		return (NULL);
	if (build_tokens(&ctx) != 0)
	{
		free(ctx.status);
		free_argv(ctx.argv);
		return (NULL);
	}
	free(ctx.status);
	compact_tokens(ctx.argv);
	return (ctx.argv);
}
