/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 10:10:15 by mananton          #+#    #+#             */
/*   Updated: 2025/09/25 10:10:21 by mananton         ###   ########.fr       */
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
	char	*w;
	size_t	len;

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

char	**split_simple_args(const char *line)
{
	size_t	i;
	size_t	j;
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
		while (line[i] && is_ws(line[i]))
			i++;
		if (!line[i])
			break ;
		j = i;
		while (line[j] && !is_ws(line[j]))
			j++;
		argv[k] = dup_word(line, i, j);
		if (!argv[k])
			return (free_argv(argv), NULL);
		k++;
		i = j;
	}
	argv[k] = NULL;
	return (argv);
}

void	free_argv(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
