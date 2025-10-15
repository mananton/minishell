/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:49:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:39:49 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "env/env.h"

# define TOKEN_META_QUOTED 0x1
# define TOKEN_META_NO_GLOB 0x2

size_t						token_len(const char *s, size_t start, size_t *end);
int							split_count_tokens(const char *line,
								size_t *out_count);
char						**split_build_argv(const char *line, size_t count,
								t_env *env);
char						**split_args_quotes(const char *line, t_env *env);

void						token_meta_register(char *str, unsigned int flags);
unsigned int				token_meta_flags(const char *str);
void						token_meta_forget(char *str);

int							expand_wildcards(char ***argv);

#endif
