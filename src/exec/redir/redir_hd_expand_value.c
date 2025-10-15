/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_expand_value.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:36:22 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:20:30 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redir_internal.h"

int	hd_copy_value(char *dst, size_t *j, const char *value)
{
	size_t	k;

	if (!value)
		return (0);
	k = 0;
	while (value[k])
	{
		dst[*j] = value[k];
		*j += 1;
		k++;
	}
	return (0);
}

size_t	hd_dollar_len(const char *s, t_hd_ctx *ctx, size_t *consumed)
{
	const char	*value;
	size_t		name_len;

	if (s[1] == '?')
	{
		*consumed = 2;
		if (!ctx->status)
			return (0);
		return (strlen(ctx->status));
	}
	if (!s[1] || !is_var_start(s[1]))
	{
		*consumed = 1;
		return (1);
	}
	name_len = hd_var_len(s + 1);
	value = hd_lookup_env(ctx->env, s + 1, name_len, ctx->err);
	*consumed = 1 + name_len;
	if (ctx->err && *(ctx->err) != 0)
		return (0);
	if (!value)
		return (0);
	return (strlen(value));
}

size_t	hd_copy_dollar(const char *s, char *dst, size_t *j, t_hd_ctx *ctx)
{
	const char	*value;
	size_t		name_len;

	if (s[1] == '?')
	{
		hd_copy_value(dst, j, ctx->status);
		return (2);
	}
	if (!s[1] || !is_var_start(s[1]))
	{
		dst[*j] = s[0];
		*j += 1;
		return (1);
	}
	name_len = hd_var_len(s + 1);
	value = hd_lookup_env(ctx->env, s + 1, name_len, ctx->err);
	if (ctx->err && *(ctx->err) != 0)
		return (1 + name_len);
	if (value)
		hd_copy_value(dst, j, value);
	return (1 + name_len);
}
