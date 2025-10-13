/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_marks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 12:45:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:27:55 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_export_mark	*find_node(t_env *env, const char *key,
							t_export_mark **prev)
{
	t_export_mark	*cur;
	t_export_mark	*last;

	if (prev)
		*prev = NULL;
	if (!env || !key || !*key)
		return (NULL);
	cur = env->export_marks;
	last = NULL;
	while (cur)
	{
		if (strcmp(cur->key, key) == 0)
		{
			if (prev)
				*prev = last;
			return (cur);
		}
		last = cur;
		cur = cur->next;
	}
	return (NULL);
}

void	export_mark_add(t_env *env, const char *key)
{
	t_export_mark	*node;

	if (!env || !key || !*key)
		return ;
	if (find_node(env, key, NULL))
		return ;
	node = (t_export_mark *)malloc(sizeof(t_export_mark));
	if (!node)
		return ;
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return ;
	}
	node->next = env->export_marks;
	env->export_marks = node;
}

void	export_mark_remove(t_env *env, const char *key)
{
	t_export_mark	*prev;
	t_export_mark	*node;

	if (!env || !key)
		return ;
	node = find_node(env, key, &prev);
	if (!node)
		return ;
	if (prev)
		prev->next = node->next;
	else
		env->export_marks = node->next;
	free(node->key);
	free(node);
}

int	export_mark_has(t_env *env, const char *key)
{
	if (!env || !key)
		return (0);
	return (find_node(env, key, NULL) != NULL);
}

void	export_mark_clear_all(t_env *env)
{
	t_export_mark	*node;

	if (!env)
		return ;
	while (env->export_marks)
	{
		node = env->export_marks;
		env->export_marks = node->next;
		free(node->key);
		free(node);
	}
}
