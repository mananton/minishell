/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:10:47 by mananton          #+#    #+#             */
/*   Updated: 2025/09/30 12:36:29 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static size_t	metachar_span(const char *s, size_t i)
{
	if ((s[i] == '<' || s[i] == '>') && s[i + 1] == s[i])
		return (2);
	return (1);
}

static int	is_dquote_escapable(char c)
{
	return (c == '\\' || c == '"' || c == '$');
}

static int	handle_escape_len(const char *s, size_t *i, size_t *len,
			int in_double)
{
	if (in_double)
	{
		if (s[*i + 1] && is_dquote_escapable(s[*i + 1]))
		{
			(*len)++;
			*i += 2;
			return (0);
		}
		(*len)++;
		*i += 1;
		return (0);
	}
	if (!s[*i + 1])
	{
		put_str_fd("minishell: syntax error: unfinished escape sequence\n", 2);
		return (1);
	}
	(*len)++;
	*i += 2;
	return (0);
}

/*
** token_len:
** - Mede o próximo token, somando o "tamanho lógico" (aspas só agrupam).
** - Em *end devolve o índice imediatamente após o token.
** - Retorna (size_t)-1 se houver aspa não fechada.
*/
size_t	token_len(const char *s, size_t start, size_t *end)
{
	size_t	i;
	size_t	len;
	int		in_single;
	int		in_double;

	if (!s || !s[start])
	{
		if (end)
			*end = start;
		return (0);
	}
	i = start;
	len = 0;
	in_single = 0;
	in_double = 0;
	if (is_metachar(s[i]))
	{
		size_t	span;

		span = metachar_span(s, i);
		if (end)
			*end = i + span;
		return (span);
	}
	while (s[i])
	{
		if (!in_single && !in_double && (is_blank(s[i]) || is_metachar(s[i])))
			break ;
		if (!in_double && s[i] == '\'')
		{
			in_single = !in_single;
			i++;
			continue ;
		}
		if (!in_single && s[i] == '"')
		{
			in_double = !in_double;
			i++;
			continue ;
		}
		if (!in_single && s[i] == '\\')
		{
			if (handle_escape_len(s, &i, &len, in_double) != 0)
				return ((size_t)-1);
			continue ;
		}
		len++;
		i++;
	}
	if (in_single || in_double)
	{
		put_str_fd("minishell: syntax error: unclosed quote\n", 2);
		return ((size_t)-1);
	}
	if (end)
		*end = i;
	return (len);
}

/*
** split_count_tokens:
** - 1ª passada: conta quantos tokens existem na linha.
** - Em erro de aspa não fechada → imprime erro e retorna -1.
*/
int	split_count_tokens(const char *line, size_t *out_count)
{
	size_t	i;
	size_t	end;
	size_t	count;
	size_t	len;

	if (!line)
		return (-1);
	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && is_blank(line[i]))
			i++;
		if (!line[i])
			break ;
		len = token_len(line, i, &end);
		if (len == (size_t)-1)
			return (-1);
		count++;
		i = end;
	}
	*out_count = count;
	return (0);
}

/*
** split_args_quotes:
** - Wrapper: conta tokens e depois constrói argv (2ª passada).
*/
char	**split_args_quotes(const char *line, t_env *env)
{
	size_t	count;

	if (split_count_tokens(line, &count) != 0)
		return (NULL);
	return (split_build_argv(line, count, env));
}
