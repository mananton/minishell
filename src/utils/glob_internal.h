/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_internal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:30:01 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:50:31 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOB_INTERNAL_H
# define GLOB_INTERNAL_H

# include "parse/lexer.h"
# include <glob.h>
# include <stddef.h>
# include <stdlib.h>

typedef struct s_expand_node
{
	char					*value;
	int						own;
	struct s_expand_node	*next;
}							t_expand_node;

typedef struct s_glob_state
{
	t_expand_node			*head;
	t_expand_node			*tail;
	char					**free_list;
	size_t					free_len;
	size_t					free_cap;
}							t_glob_state;

int							glob_contains_wildcards(const char *s);
int							glob_add_to_free(char ***arr, size_t *len,
								size_t *cap, char *ptr);
int							glob_append_node(t_expand_node **head,
								t_expand_node **tail, char *value, int own);
void						glob_free_node_list(t_expand_node *node,
								int release_values);
int							glob_build_match_nodes(glob_t *gl,
								t_expand_node **out_head,
								t_expand_node **out_tail);
int							glob_fetch_matches(const char *pattern,
								t_expand_node **out_head,
								t_expand_node **out_tail);
void						glob_register_node_meta(t_expand_node *node);
size_t						glob_count_nodes(t_expand_node *node);
void						glob_copy_nodes(char **dst, t_expand_node *node);

#endif
