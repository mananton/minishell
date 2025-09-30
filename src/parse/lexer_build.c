/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:12:27 by mananton          #+#    #+#             */
/*   Updated: 2025/09/30 12:46:15 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* helper: copia o conteúdo entre aspas (simples ou duplas) para out
** - s[i] deve apontar para a aspa de abertura
** - devolve o novo i (logo após a aspa de fecho) */
static size_t	copy_quoted(const char *s, size_t i, size_t end, char *out, size_t *j)
{
	char	q;

	q = s[i++];
	while (i < end && s[i] != q)
		out[(*j)++] = s[i++];
	if (i < end)
		i++; /* salta a aspa de fecho */
	return (i);
}

/*
** read_token:
** - Copia o token removendo aspas simples e duplas (sem escapes/expansões).
** - Assume que token_len já devolveu len/end válidos (len = nº de chars úteis).
*/
char	*read_token(const char *s, size_t start, size_t end, size_t len)
{
	char	*out;
	size_t	i;
	size_t	j;

	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	i = start;
	j = 0;
	while (i < end)
	{
		if (s[i] == '\'' || s[i] == '\"')
			i = copy_quoted(s, i, end, out, &j);
		else
			out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}

/* avança espaços/tabs a partir de i e devolve novo índice */
static size_t	skip_blanks(const char *s, size_t i)
{
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (i);
}

/*
** split_build_argv:
** - 2ª passada: aloca argv[count+1] e preenche cada token.
** - Em erro (aspa não fechada ou malloc), liberta argv e retorna NULL.
*/
char	**split_build_argv(const char *line, size_t count)
{
	size_t	i;
	size_t	k;
	size_t	end;
	size_t	len;
	char	**argv;

	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = skip_blanks(line, 0);
	k = 0;
	while (line[i])
	{
		len = token_len(line, i, &end);
		if (len == (size_t)-1)
			return (free_argv(argv), put_str_fd("minishell: syntax error: unclosed quote\n", 2), NULL);
		argv[k] = read_token(line, i, end, len);
		if (!argv[k++])
			return (free_argv(argv), NULL);
		i = skip_blanks(line, end);
	}
	argv[k] = NULL;
	return (argv);
}
