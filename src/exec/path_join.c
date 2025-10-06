/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_join.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:43:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 12:52:22 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/* local: strlen simples (não exposta) */
static size_t	ft_strlen_(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

/* aloca buffer para dir(sem '\0') + '/' + cmd + '\0' */
static char	*join_alloc(size_t dir_len, const char *cmd)
{
	size_t	clen;

	clen = ft_strlen_(cmd);
	return ((char *)malloc(dir_len + 1 + clen + 1));
}

/* preenche o buffer com dir[0..dir_len-1] + '/' + cmd */
static void	join_fill(char *dst, const char *dir, size_t dir_len, const char *cmd)
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

/* junta dir + "/" + cmd em novo malloc (API interna compartilhada) */
char	*path_join_seg(const char *dir, size_t dir_len, const char *cmd)
{
	char	*p;

	p = join_alloc(dir_len, cmd);
	if (!p)
		return (NULL);
	join_fill(p, dir, dir_len, cmd);
	return (p);
}