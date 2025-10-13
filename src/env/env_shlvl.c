/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_shlvl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 11:42:06 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:23:07 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_shlvl(const char *s)
{
	int	lvl;
	int	ok;

	if (!s || !*s)
		return (1);
	lvl = ft_atoi_strict(s, &ok);
	if (!ok)
		return (1);
	return (lvl + 1);
}

static int	normalize_shlvl(int lvl)
{
	if (lvl < 0)
		return (0);
	if (lvl > 1000)
		return (1);
	return (lvl);
}

int	env_fix_shlvl(t_env *env)
{
	int		lvl;
	char	*tmp;

	if (!env)
		return (1);
	lvl = parse_shlvl(env_get(env, "SHLVL"));
	lvl = normalize_shlvl(lvl);
	tmp = ft_itoa(lvl);
	if (!tmp)
		return (1);
	env_set(env, "SHLVL", tmp);
	free(tmp);
	return (0);
}
