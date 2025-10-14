/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_simple_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:55:21 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:00:43 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

int	skip_blanks_simple(const char *line)
{
	int	index;

	index = 0;
	while (line[index] && is_blank(line[index]))
		index++;
	return (index);
}

int	skip_token_simple(const char *line, int index)
{
	int	in_single;
	int	in_double;

	in_single = 0;
	in_double = 0;
	while (line[index])
	{
		if (!in_double && line[index] == '\'')
			in_single = !in_single;
		else if (!in_single && line[index] == '"')
			in_double = !in_double;
		else if (!in_single && !in_double && is_blank(line[index]))
			break ;
		index++;
	}
	if (in_single || in_double)
		return (-1);
	return (index);
}

int	count_tokens(const char *line)
{
	int	index;
	int	count;
	int	next;

	index = skip_blanks_simple(line);
	count = 0;
	while (line[index])
	{
		next = skip_token_simple(line, index);
		if (next < 0)
			return (-1);
		count++;
		index = next + skip_blanks_simple(line + next);
	}
	return (count);
}

size_t	token_len(const char *s, size_t start, size_t *end)
{
	int	next;

	next = skip_token_simple(s, (int)start);
	if (next < 0)
		return ((size_t) - 1);
	if (end)
		*end = (size_t)next;
	return ((size_t)next - start);
}

int	split_count_tokens(const char *line, size_t *out_count)
{
	int	count;

	count = count_tokens(line);
	if (count < 0)
		return (1);
	if (out_count)
		*out_count = (size_t)count;
	return (0);
}
