/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_join.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:43:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 11:09:42 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static size_t	ft_strlen_(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

static char	*join_alloc(size_t dir_len, const char *cmd)
{
	size_t	clen;

	clen = ft_strlen_(cmd);
	return ((char *)malloc(dir_len + 1 + clen + 1));
}

static void	join_fill(char *dst, const char *dir, size_t dir_len,
		const char *cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < dir_len)
	{
		dst[i] = dir[i];
		i++;
	}
	dst[i++] = '/';
	j = 0;
	while (cmd[j])
	{
		dst[i + j] = cmd[j];
		j++;
	}
	dst[i + j] = '\0';
}

char	*path_join_seg(const char *dir, size_t dir_len, const char *cmd)
{
	char	*p;

	p = join_alloc(dir_len, cmd);
	if (!p)
		return (NULL);
	join_fill(p, dir, dir_len, cmd);
	return (p);
}
