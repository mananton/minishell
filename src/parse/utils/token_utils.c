/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:33:05 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 12:54:01 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static t_token_meta	**token_meta_head(void)
{
	static t_token_meta	*head = NULL;

	return (&head);
}

void	token_meta_forget(char *str)
{
	t_token_meta	**prev;
	t_token_meta	*cur;

	if (!str)
		return ;
	prev = token_meta_head();
	cur = *prev;
	while (cur)
	{
		if (cur->str == str)
		{
			*prev = cur->next;
			free(cur);
			return ;
		}
		prev = &cur->next;
		cur = cur->next;
	}
}

void	token_meta_register(char *str, unsigned int flags)
{
	t_token_meta	*node;

	if (!str)
		return ;
	token_meta_forget(str);
	node = (t_token_meta *)malloc(sizeof(t_token_meta));
	if (!node)
		return ;
	node->str = str;
	node->flags = flags;
	node->next = *token_meta_head();
	*token_meta_head() = node;
}

unsigned int	token_meta_flags(const char *str)
{
	t_token_meta	*cur;

	cur = *token_meta_head();
	while (cur)
	{
		if (cur->str == str)
			return (cur->flags);
		cur = cur->next;
	}
	return (0);
}
