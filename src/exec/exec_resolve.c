/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:45:08 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 12:50:09 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>   /* access */

/* procura '/' manualmente (evita strchr) */
static int	has_slash(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

/* se tem '/', tenta o caminho fornecido; senão procura no PATH */
char	*resolve_path(const char *cmd, t_env *env)
{
	if (!cmd || !*cmd)
		return (NULL);
	if (has_slash(cmd))
        return (ft_strdup(cmd));
	return (find_in_path(cmd, env));
}