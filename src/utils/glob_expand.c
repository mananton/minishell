/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:05:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:38:50 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <glob.h>
#include <string.h>

static int	contains_wildcards(const char *s)
{
	while (s && *s)
	{
		if (*s == '*' || *s == '?' || *s == '[')
			return (1);
		s++;
	}
	return (0);
}

static int	add_to_free(char ***arr, size_t *len, size_t *cap, char *ptr)
{
	char	**tmp;
	size_t	new_cap;

	if (*len + 1 <= *cap)
	{
		(*arr)[(*len)++] = ptr;
		return (0);
	}
	new_cap = (*cap == 0) ? 8 : *cap * 2;
	tmp = (char **)malloc(sizeof(char *) * new_cap);
	if (!tmp)
		return (1);
	if (*len > 0)
		memcpy(tmp, *arr, sizeof(char *) * (*len));
	free(*arr);
	*arr = tmp;
	*cap = new_cap;
	(*arr)[(*len)++] = ptr;
	return (0);
}

static void	free_node_list(t_expand_node *node)
{
	t_expand_node	*next;

	while (node)
	{
		next = node->next;
		if (node->own)
		{
			token_meta_forget(node->value);
			free(node->value);
		}
		free(node);
		node = next;
	}
}

static void	free_nodes_shallow(t_expand_node *node)
{
	t_expand_node	*next;

	while (node)
	{
		next = node->next;
		free(node);
		node = next;
	}
}

static int	append_node(t_expand_node **head, t_expand_node **tail,
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

static int	build_match_nodes(glob_t *gl, t_expand_node **out_head,
		t_expand_node **out_tail)
{
	char		*dup;
	size_t		idx;
	t_expand_node	*head;
	t_expand_node	*tail;

	if (gl->gl_pathc == 0)
		return (0);
	head = NULL;
	tail = NULL;
	idx = 0;
	while (idx < gl->gl_pathc)
	{
		dup = ft_strdup(gl->gl_pathv[idx]);
		if (!dup)
		{
			free_node_list(head);
			return (-1);
		}
		if (append_node(&head, &tail, dup, 1) != 0)
		{
			free(dup);
			free_node_list(head);
			return (-1);
		}
		idx++;
	}
	*out_head = head;
	*out_tail = tail;
	return ((int)gl->gl_pathc);
}

int	expand_wildcards(char ***argv)
{
	t_expand_node	*head;
	t_expand_node	*tail;
	char			**old;
	char			**newv;
	char			**to_free;
	size_t		free_len;
	size_t		free_cap;
	size_t		count;
	size_t		idx;

	if (!argv || !*argv)
		return (0);
	old = *argv;
	head = NULL;
	tail = NULL;
	to_free = NULL;
	free_len = 0;
	free_cap = 0;
	idx = 0;
	while (old[idx])
	{
		char			*tok;
		unsigned int	flags;
		glob_t		gl;
		t_expand_node	*tmp_head;
		t_expand_node	*tmp_tail;
		int			matches;

		tok = old[idx];
		flags = token_meta_flags(tok);
		if (!(flags & (TOKEN_META_QUOTED | TOKEN_META_NO_GLOB))
			&& contains_wildcards(tok))
		{
			matches = 0;
			memset(&gl, 0, sizeof(gl));
			if (glob(tok, 0, NULL, &gl) == 0)
				matches = build_match_nodes(&gl, &tmp_head, &tmp_tail);
			else
				matches = 0;
			globfree(&gl);
			if (matches > 0)
			{
				if (add_to_free(&to_free, &free_len, &free_cap, tok) != 0)
				{
					free_node_list(tmp_head);
					matches = 0;
				}
				else
				{
					if (tail)
						tail->next = tmp_head;
					else
						head = tmp_head;
					tail = tmp_tail;
					for (t_expand_node *cur = tmp_head; cur; cur = cur->next)
						token_meta_register(cur->value, 0);
					idx++;
					continue ;
				}
			}
		}
		if (append_node(&head, &tail, tok, 0) != 0)
		{
			free_node_list(head);
			free(to_free);
			return (1);
		}
		idx++;
	}
	count = 0;
	{
		t_expand_node	*cur = head;

		while (cur)
		{
			count++;
			cur = cur->next;
		}
	}
	newv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!newv)
	{
		free_node_list(head);
		free(to_free);
		return (1);
	}
	idx = 0;
	{
		t_expand_node	*cur = head;

		while (cur)
		{
			newv[idx++] = cur->value;
			cur = cur->next;
		}
	}
	newv[idx] = NULL;
	free_nodes_shallow(head);
	free(*argv);
	*argv = newv;
	while (free_len > 0)
	{
		char *ptr = to_free[--free_len];
		token_meta_forget(ptr);
		free(ptr);
	}
	free(to_free);
	return (0);
}
