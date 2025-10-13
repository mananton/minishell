/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_marks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 12:45:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/11 12:45:00 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_export_mark
{
	char			*key;
	struct s_export_mark	*next;
}t_export_mark;

static t_export_mark	*g_export_marks = NULL;

static t_export_mark	*find_node(const char *key, t_export_mark **prev)
{
	t_export_mark	*cur;
	t_export_mark	*last;

	cur = g_export_marks;
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
	if (prev)
		*prev = NULL;
	return (NULL);
}

void	export_mark_add(const char *key)
{
	t_export_mark	*node;

	if (!key || !*key)
		return ;
	if (find_node(key, NULL))
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
	node->next = g_export_marks;
	g_export_marks = node;
}

void	export_mark_remove(const char *key)
{
	t_export_mark	*prev;
	t_export_mark	*node;

	if (!key)
		return ;
	node = find_node(key, &prev);
	if (!node)
		return ;
	if (prev)
		prev->next = node->next;
	else
		g_export_marks = node->next;
	free(node->key);
	free(node);
}

int	export_mark_has(const char *key)
{
	if (!key)
		return (0);
	return (find_node(key, NULL) != NULL);
}

void	export_mark_clear_all(void)
{
	t_export_mark	*node;

	while (g_export_marks)
	{
		node = g_export_marks;
		g_export_marks = node->next;
		free(node->key);
		free(node);
	}
}
