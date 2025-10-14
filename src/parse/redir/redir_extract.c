/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_extract.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:20:41 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:28:21 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_parse_internal.h"
#include <stdlib.h>

int	append_redir_token(char **av, size_t idx, t_redir *r)
{
	int	flag;

	if (is_token_exact(av[idx], "<<"))
	{
		flag = ((token_meta_flags(av[idx + 1]) & TOKEN_META_QUOTED) == 0);
		return (redir_append(r, REDIR_HEREDOC, av[idx + 1], flag));
	}
	if (is_token_exact(av[idx], "<"))
		return (redir_append(r, REDIR_IN, av[idx + 1], 0));
	if (is_token_exact(av[idx], ">"))
		return (redir_append(r, REDIR_OUT_TRUNC, av[idx + 1], 0));
	if (is_token_exact(av[idx], ">>"))
		return (redir_append(r, REDIR_OUT_APPEND, av[idx + 1], 0));
	return (0);
}

int	append_redir_safe(char **av, size_t idx, t_redir *r)
{
	if (append_redir_token(av, idx, r) != 0)
	{
		put_str_fd("minishell: malloc failed\n", 2);
		return (1);
	}
	return (0);
}

int	extract_redirs(char **av, t_redir *r)
{
	size_t	idx;

	idx = 0;
	while (av && av[idx])
	{
		if (is_redir_operator(av[idx]) && !av[idx + 1])
		{
			put_str_fd("minishell: expected filename after redirection\n",
				2);
			return (1);
		}
		if (is_redir_operator(av[idx]) && av[idx + 1]
			&& append_redir_safe(av, idx, r) != 0)
			return (1);
		if (is_redir_operator(av[idx]) && av[idx + 1])
			idx += 2;
		else
			idx++;
	}
	return (0);
}
