/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:48:34 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:36:51 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_H
# define PIPELINE_H

# include <stddef.h>
# include "redir/redir.h"
# include "env/env.h"

typedef struct s_cmd
{
	char					**argv;
	t_redir					redir;
}							t_cmd;

int							split_pipeline(char **argv, char ****segs,
								int *count);
void						free_segments(char ****segs, int count);
int							build_cmdlist(char ***segs, int count,
								t_cmd **out_cmds);
void						free_cmdlist(t_cmd *cmds, int count);
int							exec_pipeline(t_cmd *cmds, int count, t_env *env);

#endif
