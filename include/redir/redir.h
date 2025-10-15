/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:47:43 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:37:22 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_H
# define REDIR_H

# include <stddef.h>
# include "env/env.h"

# define HDOC_INTERRUPTED 130

# define REDIR_IN 1
# define REDIR_HEREDOC 2
# define REDIR_OUT_TRUNC 3
# define REDIR_OUT_APPEND 4

typedef struct s_redir_step
{
	int						kind;
	char					*word;
	int						flag;
}							t_redir_step;

typedef struct s_redir
{
	t_redir_step			*steps;
	size_t					count;
}							t_redir;

typedef struct s_parent_redir
{
	int						saved_in;
	int						saved_out;
	int						fd_in;
	int						fd_out;
}							t_parent_redir;

int							parse_redirs(char **argv_in, char ***argv_out,
								t_redir *r);
int							open_redirs(const t_redir *r, int *fd_in,
								int *fd_out, t_env *env);
int							apply_redirs_parent(const t_redir *r,
								t_parent_redir *io, t_env *env);
void						restore_stdio_parent(int saved_in, int saved_out,
								int fd_in, int fd_out);
void						redir_clear(t_redir *r);

#endif
