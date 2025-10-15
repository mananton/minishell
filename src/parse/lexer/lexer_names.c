/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_names.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:46:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:00:22 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer_internal.h"

char	*dup_var_name(const char *s, size_t len)
{
	char	*name;
	size_t	i;

	name = (char *)malloc(len + 1);
	if (!name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		name[i] = s[i];
		i++;
	}
	name[len] = '\0';
	return (name);
}

size_t	var_value_len(t_env *env, const char *s, size_t len, int *err)
{
	char		*name;
	const char	*value;

	name = dup_var_name(s, len);
	if (!name)
	{
		*err = 1;
		return (0);
	}
	value = env_get(env, name);
	free(name);
	if (!value)
		return (0);
	return (strlen(value));
}

int	copy_var_value(t_emit *emit, t_env *env, const char *s, size_t len)
{
	char		*name;
	const char	*value;
	size_t		k;

	name = dup_var_name(s, len);
	if (!name)
		return (1);
	value = env_get(env, name);
	free(name);
	if (!value)
		return (0);
	k = 0;
	while (value[k])
	{
		emit->out[*emit->pos] = value[k];
		*emit->pos += 1;
		k++;
	}
	return (0);
}

void	copy_status(char *out, size_t *j, const char *status)
{
	size_t	k;

	k = 0;
	while (status[k])
		out[(*j)++] = status[k++];
}
