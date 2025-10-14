/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parse_internal.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:18:52 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:37:37 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_PARSE_INTERNAL_H
# define REDIR_PARSE_INTERNAL_H

# include "minishell.h"

int		is_token_exact(char *s, const char *pattern);
int		is_redir_operator(char *s);
void	redir_reset(t_redir *r);
int		redir_append(t_redir *r, int kind, char *word, int flag);

size_t	count_argv_clean(char **av);
int		dup_clean_token(char **out, size_t *k, char *token, char *next);
char	**build_clean_argv(char **av, size_t keep);

int		append_redir_token(char **av, size_t idx, t_redir *r);
int		append_redir_safe(char **av, size_t idx, t_redir *r);
int		extract_redirs(char **av, t_redir *r);

int		parse_redirs(char **argv_in, char ***argv_out, t_redir *r);
void	redir_clear(t_redir *r);

#endif
