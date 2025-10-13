/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:32:23 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:32:23 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export_internal.h"

static void	print_escaped_value(const char *value)
{
	size_t	i;

	if (!value)
		return ;
	i = 0;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\')
			write(1, "\\", 1);
		write(1, &value[i], 1);
		i++;
	}
}

static int	is_mark_only_entry(t_env *env, const char *entry)
{
	const char	*eq;
	char		*key;
	int			res;

	eq = strchr(entry, '=');
	if (!eq || eq[1] != '\0')
		return (0);
	key = dup_n(entry, (size_t)(eq - entry));
	if (!key)
		return (0);
	res = export_mark_has(env, key);
	free(key);
	return (res);
}

static void	print_one_export(t_env *env, const char *entry)
{
	const char	*eq;
	size_t		key_len;

	if (!entry)
		return ;
	put_str_fd("declare -x ", 1);
	eq = strchr(entry, '=');
	if (!eq)
	{
		put_str_fd(entry, 1);
		put_str_fd("\n", 1);
		return ;
	}
	key_len = (size_t)(eq - entry);
	write(1, entry, key_len);
	if (is_mark_only_entry(env, entry))
	{
		put_str_fd("\n", 1);
		return ;
	}
	put_str_fd("=\"", 1);
	print_escaped_value(eq + 1);
	put_str_fd("\"\n", 1);
}

static void	export_print_sorted(t_env *env, char **sorted, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		print_one_export(env, sorted[i]);
		i++;
	}
}

int	print_sorted_export(t_env *env)
{
	size_t	count;
	char	**sorted;

	if (!env || !env->vars)
		return (1);
	count = env_count(env);
	sorted = (char **)malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (put_str_fd("minishell: export: malloc failed\n", 2), 1);
	export_copy_env_refs(sorted, env->vars, count);
	sort_ascii(sorted, count);
	export_print_sorted(env, sorted, count);
	free(sorted);
	return (0);
}
