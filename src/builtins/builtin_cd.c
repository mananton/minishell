/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:03:35 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 13:51:54 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	resolve_cd_destination(char **args, t_env *env, const char **dest,
		int *should_print)
{
	*should_print = 0;
	if (args[1] && strcmp(args[1], "-") == 0)
	{
		*dest = env_get(env, "OLDPWD");
		if (!*dest)
			return (put_str_fd("minishell: cd: OLDPWD not set\n", 2), 1);
		*should_print = 1;
		return (0);
	}
	if (args[1])
	{
		*dest = args[1];
		return (0);
	}
	*dest = env_get(env, "HOME");
	if (!*dest)
		return (put_str_fd("minishell: cd: HOME not set\n", 2), 1);
	return (0);
}

static int	update_cd_env_and_print(const char *old_dir, t_env *env,
		int show_path)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (perror("minishell: cd: getcwd"), 0);
	if (old_dir)
		(void)env_set(env, "OLDPWD", old_dir);
	(void)env_set(env, "PWD", current_dir);
	if (show_path)
	{
		put_str_fd(current_dir, 1);
		put_str_fd("\n", 1);
	}
	free(current_dir);
	return (0);
}

/*
** builtin_cd:
** - Usa resolve_cd_destination para obter o destino (suporta "cd -").
** - Faz chdir(); em sucesso:
**     OLDPWD <- PWD antigo
**     PWD    <- getcwd()
**     se "cd -" → imprime PWD novo.
*/

int	builtin_cd(char **argv, t_env *env)
{
	const char	*dest_dir;
	int			show_path;
	const char	*prev_pwd;

	if (argv && argv[1] && argv[2])
	{
		put_str_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (resolve_cd_destination(argv, env, &dest_dir, &show_path))
		return (1);
	prev_pwd = env_get(env, "PWD");
	if (chdir(dest_dir) != 0)
		return (perror("minishell: cd"), 1);
	return (update_cd_env_and_print(prev_pwd, env, show_path));
}
