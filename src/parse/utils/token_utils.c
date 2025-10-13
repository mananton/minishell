/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 12:33:05 by mananton          #+#    #+#             */
/*   Updated: 2025/09/30 12:33:26 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

typedef struct s_token_meta
{
	char				*str;
	unsigned int		flags;
	struct s_token_meta	*next;
}	t_token_meta;

static t_token_meta	*g_token_meta = NULL;

void	token_meta_forget(char *str)
{
	t_token_meta	**prev;
	t_token_meta	*cur;

	if (!str)
		return ;
	prev = &g_token_meta;
	cur = g_token_meta;
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
	node->next = g_token_meta;
	g_token_meta = node;
}

unsigned int	token_meta_flags(const char *str)
{
	t_token_meta	*cur;

	cur = g_token_meta;
	while (cur)
	{
		if (cur->str == str)
			return (cur->flags);
		cur = cur->next;
	}
	return (0);
}
