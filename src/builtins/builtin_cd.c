/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:03:35 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 13:12:09 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** cd_resolve_target:
** Decide para onde ir:
** - argv[1] == "-"  -> usa OLDPWD e marca print_path=1
** - argv[1] existe  -> usa argv[1]
** - sem argv[1]     -> usa HOME
** Retorna 0 em sucesso, 1 em erro (mensagem já impressa).
*/
static int	cd_resolve_target(char **argv, t_env *env,
								const char **path, int *print_path)
{
	*print_path = 0;
	if (argv[1] && strcmp(argv[1], "-") == 0)
	{
		*path = env_get(env, "OLDPWD");
		if (!*path)
			return (put_str_fd("minishell: cd: OLDPWD not set\n", 2), 1);
		*print_path = 1;
		return (0);
	}
	if (argv[1])
		return (*path = argv[1], 0);
	*path = env_get(env, "HOME");
	if (!*path)
		return (put_str_fd("minishell: cd: HOME not set\n", 2), 1);
	return (0);
}

/*
** builtin_cd:
** - Usa cd_resolve_target para obter o destino (suporta "cd -").
** - Faz chdir(); em sucesso:
**     OLDPWD <- PWD antigo
**     PWD    <- getcwd()
**     se "cd -" → imprime PWD novo.
*/
int	builtin_cd(char **argv, t_env *env)
{
	const char	*path;
	const char	*old_pwd;
	char		*new_pwd;
	int			print_path;

	if (argv && argv[1] && argv[2])
	{
		put_str_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (cd_resolve_target(argv, env, &path, &print_path))
		return (1);
	old_pwd = env_get(env, "PWD");
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (perror("minishell: cd: getcwd"), 0);
	if (old_pwd)
		(void)env_set(env, "OLDPWD", old_pwd);
	(void)env_set(env, "PWD", new_pwd);
	if (print_path)
	{
		put_str_fd(new_pwd, 1);
		put_str_fd("\n", 1);
	}
	free(new_pwd);
	return (0);
}
