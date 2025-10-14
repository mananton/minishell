/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:21:16 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 16:21:16 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_parse_internal.h"
#include <stdlib.h>

int	parse_redirs(char **argv_in, char ***argv_out, t_redir *r)
{
	size_t	keep;

	if (!argv_in || !argv_in[0] || !argv_out || !r)
		return (1);
	*argv_out = NULL;
	redir_reset(r);
	if (extract_redirs(argv_in, r) != 0)
	{
		redir_clear(r);
		return (1);
	}
	keep = count_argv_clean(argv_in);
	*argv_out = build_clean_argv(argv_in, keep);
	if (!*argv_out)
	{
		redir_clear(r);
		put_str_fd("minishell: malloc failed\n", 2);
		return (1);
	}
	return (0);
}

void	redir_clear(t_redir *r)
{
	size_t	i;

	if (!r)
		return ;
	if (r->steps)
	{
		i = 0;
		while (i < r->count)
		{
			free(r->steps[i].word);
			i++;
		}
	}
	free(r->steps);
	r->steps = NULL;
	r->count = 0;
}
