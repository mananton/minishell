/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_internal.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:32:11 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:32:11 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_EXPORT_INTERNAL_H
# define BUILTIN_EXPORT_INTERNAL_H

# include "minishell.h"

char				*dup_n(const char *s, size_t n);
const char			*find_equal(const char *s);
size_t				env_count(t_env *env);
void				sort_ascii(char **arr, size_t count);
void				export_copy_env_refs(char **dest, char **src, size_t count);

int					print_sorted_export(t_env *env);

int					export_assign(t_env *env, const char *arg);
int					export_mark(t_env *env, const char *ident);
int					export_process_argument(t_env *env, const char *arg);

#endif
