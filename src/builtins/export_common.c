/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_common.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:32:17 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:58:33 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export_internal.h"

char	*dup_n(const char *s, size_t n)
{
	char	*out;
	size_t	i;

	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < n)
	{
		out[i] = s[i];
		i++;
	}
	out[n] = '\0';
	return (out);
}

const char	*find_equal(const char *s)
{
	while (s && *s && *s != '=')
		s++;
	if (s && *s == '=')
		return (s);
	return (NULL);
}

size_t	env_count(t_env *env)
{
	size_t	count;

	count = 0;
	while (env && env->vars && env->vars[count])
		count++;
	return (count);
}

void	sort_ascii(char **arr, size_t count)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	export_copy_env_refs(char **dest, char **src, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		dest[i] = src[i];
		i++;
	}
	dest[count] = NULL;
}
