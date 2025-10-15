/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:05:00 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:11:07 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob_internal.h"

static void	release_free_list(t_glob_state *st)
{
	char	*ptr;

	while (st->free_len > 0)
	{
		st->free_len--;
		ptr = st->free_list[st->free_len];
		token_meta_forget(ptr);
		free(ptr);
	}
	free(st->free_list);
	st->free_list = NULL;
	st->free_cap = 0;
}

static int	attach_matches(t_glob_state *st, t_expand_node *match_head,
		t_expand_node *match_tail, char *orig)
{
	if (glob_add_to_free(&st->free_list, &st->free_len, &st->free_cap, orig)
		!= 0)
		return (1);
	if (st->tail)
		st->tail->next = match_head;
	else
		st->head = match_head;
	st->tail = match_tail;
	glob_register_node_meta(match_head);
	return (0);
}

static int	process_token(t_glob_state *st, char *tok)
{
	unsigned int	flags;
	t_expand_node	*match_head;
	t_expand_node	*match_tail;
	int				matches;

	flags = token_meta_flags(tok);
	if ((flags & (TOKEN_META_QUOTED | TOKEN_META_NO_GLOB)) != 0)
		return (glob_append_node(&st->head, &st->tail, tok, 0));
	if (!glob_contains_wildcards(tok))
		return (glob_append_node(&st->head, &st->tail, tok, 0));
	match_head = NULL;
	match_tail = NULL;
	matches = glob_fetch_matches(tok, &match_head, &match_tail);
	if (matches <= 0)
		return (glob_append_node(&st->head, &st->tail, tok, 0));
	if (attach_matches(st, match_head, match_tail, tok) != 0)
	{
		glob_free_node_list(match_head, 1);
		return (glob_append_node(&st->head, &st->tail, tok, 0));
	}
	return (0);
}

static int	finalize_state(t_glob_state *st, char ***argv)
{
	size_t	count;
	char	**newv;

	count = glob_count_nodes(st->head);
	newv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!newv)
		return (1);
	glob_copy_nodes(newv, st->head);
	newv[count] = NULL;
	glob_free_node_list(st->head, 0);
	free(*argv);
	*argv = newv;
	return (0);
}

int	expand_wildcards(char ***argv)
{
	t_glob_state	state;
	size_t			idx;
	char			**old;

	if (!argv || !*argv)
		return (0);
	state.head = NULL;
	state.tail = NULL;
	state.free_list = NULL;
	state.free_len = 0;
	state.free_cap = 0;
	old = *argv;
	idx = 0;
	while (old[idx] && process_token(&state, old[idx]) == 0)
		idx++;
	if (old[idx] || finalize_state(&state, argv) != 0)
	{
		glob_free_node_list(state.head, 1);
		release_free_list(&state);
		return (1);
	}
	release_free_list(&state);
	return (0);
}
