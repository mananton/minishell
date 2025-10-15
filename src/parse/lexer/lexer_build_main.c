/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build_main.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:49:35 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:01:11 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

char	*build_token(const t_expand_ctx *base, size_t start, int *quoted_any)
{
	t_token_build	build;

	if (token_build_prepare(&build, base, start, quoted_any) != 0)
		return (NULL);
	if (build_token_body(&build.ctx, &build.copy, &build.emit) != 0)
	{
		free(build.emit.out);
		return (NULL);
	}
	if (*quoted_any)
		build.copy.inhibit_glob = 0;
	token_meta_register(build.emit.out,
		token_flags(*quoted_any, build.copy.inhibit_glob));
	return (build.emit.out);
}

int	init_build_ctx(t_build_ctx *ctx, const char *line, size_t count,
		t_env *env)
{
	ctx->line = line;
	ctx->count = count;
	ctx->env = env;
	ctx->arg_index = 0;
	ctx->index = 0;
	ctx->status = ft_itoa(env->last_status);
	if (!ctx->status)
		return (1);
	ctx->status_len = strlen(ctx->status);
	ctx->argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!ctx->argv)
	{
		free(ctx->status);
		ctx->status = NULL;
		return (1);
	}
	return (0);
}

void	compact_tokens(char **argv)
{
	size_t	read;
	size_t	write;

	read = 0;
	write = 0;
	while (argv[read])
	{
		if (argv[read][0] == '\0'
			&& (token_meta_flags(argv[read]) & TOKEN_META_QUOTED) == 0)
		{
			token_meta_forget(argv[read]);
			free(argv[read]);
		}
		else
		{
			argv[write] = argv[read];
			write++;
		}
		read++;
	}
	argv[write] = NULL;
}

size_t	skip_blanks(const char *s, size_t i)
{
	while (s[i] && is_blank(s[i]))
		i++;
	return (i);
}
