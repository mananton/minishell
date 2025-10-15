/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 13:59:31 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:59:25 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipeline_internal.h"
#include <stdlib.h>

static int	is_pipe_token(char *s)
{
	return (s && s[0] == '|' && s[1] == '\0'
		&& (token_meta_flags(s) & TOKEN_META_QUOTED) == 0);
}

int	count_pipes(char **av)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (av && av[i])
	{
		if (is_pipe_token(av[i]))
			c++;
		i++;
	}
	return (c);
}

static int	seg_len(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		if (is_pipe_token(av[i]))
			break ;
		i++;
	}
	return (i);
}

static int	fill_one_segment(char **argv, int pos, char ***dst, int *next_pos)
{
	int		len;
	int		j;

	len = seg_len(argv + pos);
	if (len == 0)
		return (1);
	*dst = (char **)malloc(sizeof(char *) * (len + 1));
	if (!*dst)
		return (1);
	j = 0;
	while (j < len)
	{
		(*dst)[j] = argv[pos + j];
		j++;
	}
	(*dst)[len] = NULL;
	pos += len;
	if (argv[pos] && is_pipe_token(argv[pos]))
		pos++;
	*next_pos = pos;
	return (0);
}

int	split_fill(char **argv, char ***tmp, int count)
{
	int	pos;
	int	i;

	pos = 0;
	i = 0;
	while (i < count)
	{
		if (fill_one_segment(argv, pos, &tmp[i], &pos) != 0)
			return (free_segments(&tmp, i), 1);
		i++;
	}
	return (0);
}
