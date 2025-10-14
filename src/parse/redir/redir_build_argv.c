/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_build_argv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:20:05 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 16:20:05 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_parse_internal.h"
#include <stdlib.h>

size_t	count_argv_clean(char **av)
{
	size_t	keep;
	size_t	idx;

	keep = 0;
	idx = 0;
	while (av && av[idx])
	{
		if (is_redir_operator(av[idx]) && av[idx + 1])
			idx += 2;
		else
		{
			keep++;
			idx++;
		}
	}
	return (keep);
}

int	dup_clean_token(char **out, size_t *k, char *token, char *next)
{
	if (is_redir_operator(token) && next)
		return (0);
	out[*k] = ft_strdup(token);
	if (!out[*k])
	{
		out[*k] = NULL;
		free_argv(out);
		return (1);
	}
	*k += 1;
	return (0);
}

char	**build_clean_argv(char **av, size_t keep)
{
	char	**out;
	size_t	idx;
	size_t	k;

	out = (char **)malloc(sizeof(char *) * (keep + 1));
	if (!out)
		return (NULL);
	idx = 0;
	k = 0;
	while (av && av[idx]
		&& dup_clean_token(out, &k, av[idx], av[idx + 1]) == 0)
	{
		if (is_redir_operator(av[idx]) && av[idx + 1])
			idx += 2;
		else
			idx++;
	}
	if (av && av[idx])
	{
		free_argv(out);
		return (NULL);
	}
	out[k] = NULL;
	return (out);
}
