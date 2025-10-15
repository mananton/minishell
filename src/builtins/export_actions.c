/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:32:29 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:58:57 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export_internal.h"

static int	export_store_assignment(t_env *env, char *key, const char *value)
{
	if (!is_valid_ident(key))
		return (-2);
	if (env_set(env, key, value) != 0)
		return (-1);
	export_mark_remove(env, key);
	return (0);
}

int	export_assign(t_env *env, const char *arg)
{
	const char	*eq;
	char		*key;
	int			res;

	eq = find_equal(arg);
	if (!eq)
		return (1);
	key = dup_n(arg, (size_t)(eq - arg));
	if (!key)
		return (put_str_fd("minishell: export: malloc failed\n", 2), -1);
	res = export_store_assignment(env, key, eq + 1);
	free(key);
	return (res);
}

int	export_mark(t_env *env, const char *ident)
{
	int	idx;

	if (!is_valid_ident(ident))
		return (-2);
	idx = env_find_index(env, ident);
	if (idx >= 0)
		return (0);
	if (env_set(env, ident, "") != 0)
		return (-1);
	export_mark_add(env, ident);
	return (0);
}

static int	export_handle_status(int rc, const char *arg)
{
	if (rc == -2)
	{
		put_str_fd("minishell: export: '", 2);
		put_str_fd(arg, 2);
		put_str_fd("': not a valid identifier\n", 2);
		return (1);
	}
	if (rc != 0)
	{
		put_str_fd("minishell: export: failed: ", 2);
		put_str_fd(arg, 2);
		put_str_fd("\n", 2);
		return (1);
	}
	return (0);
}

int	export_process_argument(t_env *env, const char *arg)
{
	int	rc;

	if (find_equal(arg))
		rc = export_assign(env, arg);
	else
		rc = export_mark(env, arg);
	return (export_handle_status(rc, arg));
}
