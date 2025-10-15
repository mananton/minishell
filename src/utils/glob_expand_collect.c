/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_expand_collect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:31:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 12:31:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob_expand_internal.h"

size_t	glob_count_nodes(t_expand_node *node)
{
	size_t	count;

	count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

void	glob_copy_nodes(char **dst, t_expand_node *node)
{
	size_t	index;

	index = 0;
	while (node)
	{
		dst[index] = node->value;
		index++;
		node = node->next;
	}
}
