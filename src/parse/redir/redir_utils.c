/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:19:31 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:02:09 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redir_internal.h"
#include <string.h>
#include <stdlib.h>

int	is_token_exact(char *s, const char *pattern)
{
	if (!s)
		return (0);
	if (strcmp(s, pattern) != 0)
		return (0);
	return ((token_meta_flags(s) & TOKEN_META_QUOTED) == 0);
}

int	is_redir_operator(char *s)
{
	return (is_token_exact(s, "<") || is_token_exact(s, "<<")
		|| is_token_exact(s, ">") || is_token_exact(s, ">>"));
}

void	redir_reset(t_redir *r)
{
	r->steps = NULL;
	r->count = 0;
}

int	redir_append(t_redir *r, int kind, char *word, int flag)
{
	t_redir_step	*tmp;
	t_redir_step	*old;
	t_redir_step	*dst;
	char			*dup;

	tmp = (t_redir_step *)malloc(sizeof(t_redir_step) * (r->count + 1));
	if (!tmp)
		return (1);
	old = r->steps;
	if (old)
		memcpy(tmp, old, sizeof(t_redir_step) * r->count);
	dup = ft_strdup(word);
	if (!dup)
	{
		free(tmp);
		return (1);
	}
	dst = &tmp[r->count];
	dst->kind = kind;
	dst->word = dup;
	dst->flag = flag;
	r->steps = tmp;
	r->count += 1;
	free(old);
	return (0);
}
