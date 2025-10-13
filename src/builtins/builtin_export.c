/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:33:48 by mananton          #+#    #+#             */
/*   Updated: 2025/10/11 12:45:00 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_n(const char *s, size_t n)
{
	char	*out;
	size_t	i;

	out = (char *)malloc(n + 1);
	if (!out)
		return (NULL);
	i = 0;
	while (i < n)
	{
		out[i] = s[i];
		i++;
	}
	out[n] = '\0';
	return (out);
}

static const char	*find_equal(const char *s)
{
	while (s && *s && *s != '=')
		s++;
	if (s && *s == '=')
		return (s);
	return (NULL);
}

static void	print_escaped_value(const char *value)
{
	size_t i;

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

static int	is_mark_only_entry(const char *entry)
{
	const char	*eq;
	char		*key;
	int		res;

	eq = strchr(entry, '=');
	if (!eq || eq[1] != '\0')
		return (0);
	key = dup_n(entry, (size_t)(eq - entry));
	if (!key)
		return (0);
	res = export_mark_has(key);
	free(key);
	return (res);
}

static void	print_one_export(const char *entry)
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
	if (is_mark_only_entry(entry))
	{
		put_str_fd("\n", 1);
		return ;
	}
	put_str_fd("=\"", 1);
	print_escaped_value(eq + 1);
	put_str_fd("\"\n", 1);
}

static size_t	env_count(t_env *env)
{
	size_t	count;

	count = 0;
	while (env && env->vars && env->vars[count])
		count++;
	return (count);
}

static void	sort_ascii(char **arr, size_t count)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	print_sorted_export(t_env *env)
{
	size_t	count;
	size_t	i;
	char	**sorted;

	if (!env || !env->vars)
		return (1);
	count = env_count(env);
	sorted = (char **)malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (put_str_fd("minishell: export: malloc failed\n", 2), 1);
	i = 0;
	while (i < count)
	{
		sorted[i] = env->vars[i];
		i++;
	}
	sorted[count] = NULL;
	sort_ascii(sorted, count);
	i = 0;
	while (i < count)
	{
		print_one_export(sorted[i]);
		i++;
	}
	free(sorted);
	return (0);
}

static int	export_assign(t_env *env, const char *arg)
{
	const char	*eq;
	char		*key;
	const char	*value;
	int		res;

	eq = find_equal(arg);
	if (!eq)
		return (1);
	key = dup_n(arg, (size_t)(eq - arg));
	if (!key)
		return (put_str_fd("minishell: export: malloc failed\n", 2), -1);
	if (!is_valid_ident(key))
	{
		free(key);
		return (-2);
	}
	value = eq + 1;
	if (env_set(env, key, value) != 0)
		res = -1;
	else
	{
		export_mark_remove(key);
		res = 0;
	}
	free(key);
	return (res);
}

static int	export_mark(t_env *env, const char *ident)
{
	int	idx;

	if (!is_valid_ident(ident))
		return (-2);
	idx = env_find_index(env, ident);
	if (idx >= 0)
		return (0);
	if (env_set(env, ident, "") != 0)
		return (-1);
	export_mark_add(ident);
	return (0);
}

int	builtin_export(t_env *env, char **argv)
{
	size_t	i;
	int		status;
	int		rc;

	if (!env || !argv)
		return (1);
	if (!argv[1])
		return (print_sorted_export(env));
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (find_equal(argv[i]))
			rc = export_assign(env, argv[i]);
		else
			rc = export_mark(env, argv[i]);
		if (rc == -2)
		{
			put_str_fd("minishell: export: '", 2);
			put_str_fd(argv[i], 2);
			put_str_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else if (rc != 0)
		{
			put_str_fd("minishell: export: failed: ", 2);
			put_str_fd(argv[i], 2);
			put_str_fd("\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}
