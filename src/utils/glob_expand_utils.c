/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_expand_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:30:35 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:55:54 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "glob_internal.h"

int	glob_contains_wildcards(const char *s)
{
	while (s && *s)
	{
		if (*s == '*' || *s == '?' || *s == '[')
			return (1);
		s++;
	}
	return (0);
}

int	glob_add_to_free(char ***arr, size_t *len, size_t *cap, char *ptr)
{
	char	**tmp;
	size_t	new_cap;

	if (*len + 1 <= *cap)
	{
		(*arr)[*len] = ptr;
		*len += 1;
		return (0);
	}
	new_cap = *cap;
	if (new_cap == 0)
		new_cap = 8;
	else
		new_cap *= 2;
	tmp = (char **)malloc(sizeof(char *) * new_cap);
	if (!tmp)
		return (1);
	if (*len > 0)
		memcpy(tmp, *arr, sizeof(char *) * (*len));
	free(*arr);
	*arr = tmp;
	*cap = new_cap;
	(*arr)[*len] = ptr;
	*len += 1;
	return (0);
}

int	glob_append_node(t_expand_node **head, t_expand_node **tail,
		char *value, int own)
{
	t_expand_node	*node;

	node = (t_expand_node *)malloc(sizeof(t_expand_node));
	if (!node)
		return (1);
	node->value = value;
	node->own = own;
	node->next = NULL;
	if (*tail)
		(*tail)->next = node;
	else
		*head = node;
	*tail = node;
	return (0);
}

void	glob_free_node_list(t_expand_node *node, int release_values)
{
	t_expand_node	*next;

	while (node)
	{
		next = node->next;
		if (release_values && node->own)
		{
			token_meta_forget(node->value);
			free(node->value);
		}
		free(node);
		node = next;
	}
}
