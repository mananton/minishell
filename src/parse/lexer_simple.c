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
	int		err;

	i = start;
	len = 0;
	err = 0;
	while (s[i] && !(s[i] == ' ' || s[i] == '\t'))
	{
		i = tkn_advance(s, i, &len, &err);
		if (err)
			return ((size_t)-1);
	}
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
	size_t	len;
	size_t	count;

	if (!line)
		return (-1);
	i = 0;
	count = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	while (line[i])
	{
		len = token_len(line, i, &end);
		if (len == (size_t)-1)
			return (put_str_fd("minishell: syntax error: unclosed quote\n", 2), -1);
		count++;
		i = end;
		while (line[i] == ' ' || line[i] == '\t')
			i++;
	}
	*out_count = count;
	return (0);
}

/*
** split_args_quotes:
** - Wrapper: conta tokens e depois constrói argv (2ª passada).
*/
char	**split_args_quotes(const char *line)
{
	size_t	count;

	if (split_count_tokens(line, &count) != 0)
		return (NULL);
	return (split_build_argv(line, count));
}
