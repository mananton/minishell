/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:33:48 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 13:27:07 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** dup_n:
** Cria uma nova string com os primeiros 'n' chars de 's'.
** Usamos para extrair a KEY (parte antes do '=') sem modificar a original.
*/
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

/*
** handle_kv:
** Recebe um argumento "KEY=VALUE" e:
** - encontra o '=';
** - duplica a KEY;
** - pega o VALUE como a parte após '=';
** - chama env_set(env, KEY, VALUE).
** Retorna 0 em sucesso, 1 em erro (sem '=' ou falta de memória).
*/
static int	handle_kv(t_env *env, const char *arg)
{
	const char	*eq;
	char		*key;
	const char	*value;

	eq = find_equal(arg);
	if (!eq || eq == arg)
		return (1);
	key = dup_n(arg, (size_t)(eq - arg));
	if (!key)
		return (1);
	if (!is_valid_ident(key))
		return (free(key), 1);
	value = eq + 1;
	if (env_set(env, key, value) != 0)
		return (free(key), 1);
	free(key);
	return (0);
}

/*
** builtin_export (mínimo):
** - Para cada argumento, tenta tratar como "KEY=VALUE".
** - Se algum não tiver '=', avisamos e marcamos erro (status=1).
** - Retornamos 0 se todos foram aceites; 1 se houve algum erro.
*/
int	builtin_export(t_env *env, char **argv)
{
	size_t	i;
	int		status;

	if (!env || !argv)
		return (1);
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (handle_kv(env, argv[i]) != 0)
		{
			put_str_fd("minishell: export: invalid: ", 2);
			put_str_fd(argv[i], 2);
			put_str_fd("\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}
