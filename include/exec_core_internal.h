/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:36:42 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 14:36:42 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_CORE_INTERNAL_H
# define EXEC_CORE_INTERNAL_H

# include "minishell.h"

int	spawn_and_exec(char *path, char **argv, t_env *env, const t_redir *redir);

#endif
