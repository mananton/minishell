/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_expand_match.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:32:01 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:54:37 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "glob_internal.h"

static int	append_match_value(glob_t *gl, size_t idx,
		t_expand_node **head, t_expand_node **tail)
{
	char	*dup;

	dup = ft_strdup(gl->gl_pathv[idx]);
	if (!dup)
		return (1);
	if (glob_append_node(head, tail, dup, 1) != 0)
	{
		free(dup);
		return (1);
	}
	return (0);
}

int	glob_build_match_nodes(glob_t *gl, t_expand_node **out_head,
		t_expand_node **out_tail)
{
	size_t			idx;
	t_expand_node	*head;
	t_expand_node	*tail;

	if (!gl || gl->gl_pathc == 0)
		return (0);
	head = NULL;
	tail = NULL;
	idx = 0;
	while (idx < gl->gl_pathc)
	{
		if (append_match_value(gl, idx, &head, &tail) != 0)
		{
			glob_free_node_list(head, 1);
			return (-1);
		}
		idx++;
	}
	*out_head = head;
	*out_tail = tail;
	return ((int)gl->gl_pathc);
}

int	glob_fetch_matches(const char *pattern, t_expand_node **out_head,
		t_expand_node **out_tail)
{
	glob_t	gl;
	int		result;

	*out_head = NULL;
	*out_tail = NULL;
	memset(&gl, 0, sizeof(gl));
	if (glob(pattern, 0, NULL, &gl) != 0)
	{
		globfree(&gl);
		return (0);
	}
	result = glob_build_match_nodes(&gl, out_head, out_tail);
	globfree(&gl);
	if (result < 0)
		return (-1);
	return (result);
}

void	glob_register_node_meta(t_expand_node *node)
{
	while (node)
	{
		token_meta_register(node->value, 0);
		node = node->next;
	}
}
