/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:49:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 11:49:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_ws(char c)
{
	return (c == ' ' || c == '\t');
}

static size_t	count_words(const char *s)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		while (s[i] && is_ws(s[i]))
			i++;
		if (!s[i])
			break ;
		n++;
		while (s[i] && !is_ws(s[i]))
			i++;
	}
	return (n);
}

static char	*dup_word(const char *s, size_t start, size_t end)
{
	size_t	len;
	char	*w;

	if (end < start)
		return (NULL);
	len = end - start;
	w = (char *)malloc(len + 1);
	if (!w)
		return (NULL);
	memcpy(w, s + start, len);
	w[len] = '\0';
	return (w);
}

static int	append_word(char **argv, const char *line, size_t *idx, size_t *k)
{
	size_t	start;

	while (line[*idx] && is_ws(line[*idx]))
		(*idx)++;
	if (!line[*idx])
		return (0);
	start = *idx;
	while (line[*idx] && !is_ws(line[*idx]))
		(*idx)++;
	argv[*k] = dup_word(line, start, *idx);
	if (!argv[*k])
		return (1);
	*k += 1;
	return (0);
}

char	**split_simple_args(const char *line)
{
	size_t	i;
	size_t	k;
	size_t	words;
	char	**argv;

	words = count_words(line);
	argv = (char **)malloc(sizeof(char *) * (words + 1));
	if (!argv)
		return (NULL);
	i = 0;
	k = 0;
	while (line && line[i])
	{
		if (append_word(argv, line, &i, &k) != 0)
			return (free_argv(argv), NULL);
	}
	argv[k] = NULL;
	return (argv);
}
