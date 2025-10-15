/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_base.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:46:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:01:23 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_dquote_escapable(char c)
{
	return (c == '\\' || c == '"' || c == '$');
}

size_t	var_name_len(const char *s, size_t pos, size_t end)
{
	size_t	len;

	len = 0;
	while (pos + len < end && is_var_char(s[pos + len]))
		len++;
	return (len);
}

int	handle_quote_char(char c, t_quote_state *quotes, int *quoted)
{
	if (!quotes->in_double && c == '\'')
	{
		quotes->in_single = !quotes->in_single;
		if (quoted)
			*quoted = 1;
		return (1);
	}
	if (!quotes->in_single && c == '"')
	{
		quotes->in_double = !quotes->in_double;
		if (quoted)
			*quoted = 1;
		return (1);
	}
	return (0);
}
