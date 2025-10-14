/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:56:48 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 15:56:48 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

char	**split_args_quotes(const char *line, t_env *env)
{
	size_t	count;

	if (!line)
		return (NULL);
	if (split_count_tokens(line, &count) != 0)
		return (NULL);
	return (split_build_argv(line, count, env));
}
