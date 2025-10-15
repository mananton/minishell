/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_expand_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:35:18 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:20:33 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "redir_internal.h"

int	is_var_start(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (c == '_');
}

int	is_var_char(char c)
{
	if (is_var_start(c))
		return (1);
	return (c >= '0' && c <= '9');
}

size_t	hd_var_len(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] && is_var_char(s[len]))
		len++;
	return (len);
}

const char	*hd_lookup_env(t_env *env, const char *name, size_t len, int *err)
{
	char		*dup;
	const char	*value;

	if (!env)
		return (NULL);
	dup = (char *)malloc(len + 1);
	if (!dup)
	{
		if (err)
			*err = 1;
		return (NULL);
	}
	memcpy(dup, name, len);
	dup[len] = '\0';
	value = env_get(env, dup);
	free(dup);
	return (value);
}
