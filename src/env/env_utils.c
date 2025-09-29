/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:30:36 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 10:30:53 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*out;

	if (!s)
		return (NULL);
	len = strlen(s);
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	memcpy(out, s, len + 1);
	return (out);
}

/*
** key_matches:
** Verifica se "entry" começa por "key=" (ex.: "PWD=").
*/
int	key_matches(const char *entry, const char *key)
{
	size_t	klen;

	if (!entry || !key)
		return (0);
	klen = strlen(key);
	if (strncmp(entry, key, klen) != 0)
		return (0);
	if (entry[klen] != '=')
		return (0);
	return (1);
}

/*
** env_find_index:
** Procura o índice da KEY no vetor env->vars ("KEY=VALUE").
** Retorna -1 se não encontrar.
*/
int	env_find_index(t_env *env, const char *key)
{
	size_t	i;

	if (!env || !env->vars || !key)
		return (-1);
	i = 0;
	while (env->vars[i])
	{
		if (key_matches(env->vars[i], key))
			return ((int)i);
		i++;
	}
	return (-1);
}

/*
** env_grow:
** Aumenta o vetor env->vars para acomodar 'add' novas entradas.
** Copia os ponteiros antigos e mantém o terminador NULL.
*/
char	**env_grow(t_env *env, size_t add)
{
	size_t	count;
	size_t	i;
	char	**bigger;

	count = 0;
	while (env->vars && env->vars[count])
		count++;
	bigger = (char **)malloc(sizeof(char *) * (count + add + 1));
	if (!bigger)
		return (NULL);
	i = 0;
	while (i < count)
	{
		bigger[i] = env->vars[i];
		i++;
	}
	bigger[count] = NULL;
	free(env->vars);
	env->vars = bigger;
	return (env->vars);
}
