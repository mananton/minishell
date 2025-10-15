/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:46:54 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:35:46 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "env/env.h"

# define MS_BUILTIN_EXIT 1143

int							is_builtin(const char *cmd);
int							run_builtin(char **argv, t_env *env);

int							builtin_pwd(t_env *env);
int							builtin_echo(t_env *env, char **argv);
int							builtin_cd(char **argv, t_env *env);
int							builtin_env(t_env *env, char **argv);
int							builtin_export(t_env *env, char **argv);
int							builtin_unset(t_env *env, char **argv);
int							builtin_exit(t_env *env, char **argv);
int							builtin_status(t_env *env, char **argv);

#endif
