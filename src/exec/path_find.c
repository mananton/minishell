/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_find.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:44:05 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 13:28:21 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>   /* access */
#include <stdlib.h>   /* free */

/* executável = path existe e tem X_OK */
static int	is_executable(const char *path)
{
	if (!path)
		return (0);
	return (access(path, X_OK) == 0);
}

/* itera segmentos do PATH: devolve [*start, *end); retorna 0 ao fim */
static int	path_next_seg(const char *path, size_t *start, size_t *end)
{
	size_t	i;

	if (!path || !path[*start])
		return (0);
	i = *start;
	while (path[i] && path[i] != ':')
		i++;
	*end = i;
	if (path[i] == ':')
		*start = i + 1;
	else
		*start = i;
	return (1);
}

/* tenta formar dir/cmd e verificar executável; retorna path mallocado ou NULL */
static char	*try_seg_cmd(const char *path, size_t a, size_t b, const char *cmd)
{
	char	*full;

	/* segmento vazio → tratar como "." (diretório atual) */
	if (b == a)
	{
		full = path_join_seg(".", 1, cmd);
		if (full && is_executable(full))
			return (full);
		free(full);
		return (NULL);
	}
	full = path_join_seg(path + a, b - a, cmd);
	if (full && is_executable(full))
		return (full);
	free(full);
	return (NULL);
}

/* API pública: procurar cmd nos diretórios de PATH da tua env */
char	*find_in_path(const char *cmd, t_env *env)
{
	const char	*path;
	size_t		a;
	size_t		b;
	char		*full;

	path = env_get(env, "PATH");
	if (!cmd || !*cmd || !path)
		return (NULL);
	a = 0;
	while (path[a])
	{
		size_t	seg_start;

		seg_start = a;
		if (!path_next_seg(path, &a, &b))
			break ;
		full = try_seg_cmd(path, seg_start, b, cmd);
		if (full)
			return (full);
	}
	return (NULL);
}