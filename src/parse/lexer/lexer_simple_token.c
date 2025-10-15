/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_simple_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:32:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:00:33 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

static int	update_quotes_simple(char c, t_quote_state *quotes)
{
	if (!quotes->in_double && c == '\'')
	{
		quotes->in_single = !quotes->in_single;
		return (1);
	}
	if (!quotes->in_single && c == '"')
	{
		quotes->in_double = !quotes->in_double;
		return (1);
	}
	return (0);
}

static int	token_stop_index(const char *line, int index, int start)
{
	if (line[index] == '<' || line[index] == '>' || line[index] == '|')
	{
		if (index == start
			&& line[index] == line[index + 1]
			&& (line[index] == '<' || line[index] == '>'))
			return (index + 2);
		if (index == start)
			return (index + 1);
		return (index);
	}
	if (is_blank(line[index]))
		return (index);
	return (-1);
}

static int	token_walk(const char *line, int index, int start,
		t_quote_state *quotes)
{
	int	stop;

	while (line[index])
	{
		if (update_quotes_simple(line[index], quotes))
		{
			index++;
			continue ;
		}
		if (!quotes->in_single && !quotes->in_double)
		{
			stop = token_stop_index(line, index, start);
			if (stop >= 0)
				return (stop);
		}
		index++;
	}
	return (index);
}

int	skip_token_simple(const char *line, int index)
{
	int				start;
	t_quote_state	quotes;
	int				result;

	start = index;
	quotes.in_single = 0;
	quotes.in_double = 0;
	result = token_walk(line, index, start, &quotes);
	if (quotes.in_single || quotes.in_double)
		return (-1);
	return (result);
}
