/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:12:27 by mananton          #+#    #+#             */
/*   Updated: 2025/09/30 12:46:15 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

static int	is_dquote_escapable(char c)
{
	return (c == '\\' || c == '"' || c == '$');
}

static int	is_var_start(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (c == '_');
}

static int	is_var_char(char c)
{
	if (is_var_start(c))
		return (1);
	return (c >= '0' && c <= '9');
}

static size_t	var_name_len(const char *s, size_t pos, size_t end)
{
	size_t	len;

	len = 0;
	while (pos + len < end && is_var_char(s[pos + len]))
		len++;
	return (len);
}

static char	*dup_var_name(const char *s, size_t len)
{
	char	*name;
	size_t	i;

	name = (char *)malloc(len + 1);
	if (!name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		name[i] = s[i];
		i++;
	}
	name[len] = '\0';
	return (name);
}

static size_t	var_value_len(t_env *env, const char *s, size_t len, int *err)
{
	char		*name;
	const char	*value;

	name = dup_var_name(s, len);
	if (!name)
	{
		*err = 1;
		return (0);
	}
	value = env_get(env, name);
	free(name);
	if (!value)
		return (0);
	return (strlen(value));
}

static int	copy_var_value(char *out, size_t *j, t_env *env,
			const char *s, size_t len)
{
	char		*name;
	const char	*value;
	size_t		k;

	name = dup_var_name(s, len);
	if (!name)
		return (1);
	value = env_get(env, name);
	free(name);
	if (!value)
		return (0);
	k = 0;
	while (value[k])
	{
		out[*j] = value[k];
		*j += 1;
		k++;
	}
	return (0);
}

static void	copy_status(char *out, size_t *j, const char *status)
{
	size_t	k;

	k = 0;
	while (status[k])
		out[(*j)++] = status[k++];
}

static int	handle_quote_char(char c, int *in_single, int *in_double,
			int *quoted)
{
	if (!*in_double && c == '\'')
	{
		*in_single = !*in_single;
		if (quoted)
			*quoted = 1;
		return (1);
	}
	if (!*in_single && c == '"')
	{
		*in_double = !*in_double;
		if (quoted)
			*quoted = 1;
		return (1);
	}
	return (0);
}

static size_t	dollar_literal_len(size_t *i)
{
	(*i)++;
	return (1);
}

static size_t	process_dollar_len(const char *s, size_t *i, size_t end,
		size_t status_len, t_env *env,
			int in_single, int *err)
{
	size_t	name_len;
	size_t	start;

	if (in_single)
		return (dollar_literal_len(i));
	if (*i + 1 >= end)
		return (dollar_literal_len(i));
	if (s[*i + 1] == '?')
	{
		*i += 2;
		return (status_len);
	}
	if (!is_var_start(s[*i + 1]))
		return (dollar_literal_len(i));
	name_len = var_name_len(s, *i + 1, end);
	start = *i + 1;
	*i += name_len + 1;
	return (var_value_len(env, s + start, name_len, err));
}

static int	dollar_literal_copy(const char *s, size_t *i, char *out, size_t *j)
{
	out[*j] = s[*i];
	*j += 1;
	*i += 1;
	return (0);
}

static int	process_dollar_copy(const char *s, size_t *i, size_t end,
		char *out, size_t *j, const char *status, t_env *env,
		int in_single)
{
	size_t	name_len;
	size_t	start;

	if (in_single)
		return (dollar_literal_copy(s, i, out, j));
	if (*i + 1 >= end)
		return (dollar_literal_copy(s, i, out, j));
	if (s[*i + 1] == '?')
	{
		copy_status(out, j, status);
		*i += 2;
		return (0);
	}
	if (!is_var_start(s[*i + 1]))
		return (dollar_literal_copy(s, i, out, j));
	name_len = var_name_len(s, *i + 1, end);
	start = *i + 1;
	if (copy_var_value(out, j, env, s + start, name_len) != 0)
		return (1);
	*i += name_len + 1;
	return (0);
}

static size_t	token_final_len(const char *s, size_t start, size_t end,
		size_t status_len, t_env *env, int *err, int *quoted_any)
{
	size_t	i;
	size_t	total;
	int		in_single;
	int		in_double;

	i = start;
	total = 0;
	in_single = 0;
	in_double = 0;
	while (i < end && *err == 0)
	{
		if (handle_quote_char(s[i], &in_single, &in_double, quoted_any))
			i++;
		else if (!in_single && s[i] == '\\')
		{
			if (in_double)
			{
				if (s[i + 1] && is_dquote_escapable(s[i + 1]))
				{
					total++;
					i += 2;
				}
				else
				{
					total++;
					i += 1;
				}
			}
			else
			{
				if (s[i + 1])
				{
					total++;
					i += 2;
				}
				else
					i += 1;
			}
		}
		else if (s[i] == '$')
			total += process_dollar_len(s, &i, end, status_len,
				env, in_single, err);
		else
		{
			total++;
			i++;
		}
	}
	return (total);
}

/*
** read_token:
** - Copia o token removendo aspas e expande "$?" e "$VAR".
*/
char	*read_token(const char *s, size_t start, size_t end, size_t len,
			const char *status, size_t status_len, t_env *env)
{
	char	*out;
	size_t	i;
	size_t	j;
	int		in_single;
	int		in_double;
	int		err;
	int		quoted_any;
	int		inhibit_glob;

	(void)len;
	err = 0;
	quoted_any = 0;
	inhibit_glob = 0;
	j = token_final_len(s, start, end, status_len, env, &err, &quoted_any);
	if (err)
		return (NULL);
	out = (char *)malloc(j + 1);
	if (!out)
		return (NULL);
	i = start;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (i < end)
	{
		if (handle_quote_char(s[i], &in_single, &in_double, NULL))
			i++;
		else if (!in_single && s[i] == '\\')
		{
			if (in_double)
			{
				if (s[i + 1] && is_dquote_escapable(s[i + 1]))
				{
					out[j++] = s[i + 1];
					i += 2;
				}
				else
				{
					out[j++] = s[i++];
				}
			}
			else
			{
				if (s[i + 1])
				{
					if (s[i + 1] == '*' || s[i + 1] == '?' || s[i + 1] == '[')
						inhibit_glob = 1;
					out[j++] = s[i + 1];
					i += 2;
				}
				else
					i += 1;
			}
		}
		else if (s[i] == '$')
		{
			if (process_dollar_copy(s, &i, end, out, &j, status, env,
					in_single) != 0)
				return (free(out), NULL);
		}
		else
			out[j++] = s[i++];
	}
	out[j] = '\0';
	if (quoted_any)
		inhibit_glob = 0;
	token_meta_register(out,
		(quoted_any ? TOKEN_META_QUOTED : 0)
		| (inhibit_glob ? TOKEN_META_NO_GLOB : 0));
	return (out);
}

/* avança espaços/tabs a partir de i e devolve novo índice */
static size_t	skip_blanks(const char *s, size_t i)
{
	while (s[i] && is_blank(s[i]))
		i++;
	return (i);
}

/*
** split_build_argv:
** - 2ª passada: aloca argv[count+1] e preenche cada token.
** - Em erro (aspa não fechada ou malloc), liberta argv e retorna NULL.
*/
char	**split_build_argv(const char *line, size_t count, t_env *env)
{
	size_t	i;
	size_t	k;
	size_t	end;
	size_t	len;
	char	**argv;
	char	*status;
	size_t	status_len;

	status = ft_itoa(env->last_status);
	if (!status)
		return (NULL);
	status_len = strlen(status);
	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (free(status), NULL);
	i = skip_blanks(line, 0);
	k = 0;
	while (line[i])
	{
		len = token_len(line, i, &end);
			if (len == (size_t)-1)
			{
				argv[k] = NULL;
				free(status);
				free_argv(argv);
				return (NULL);
			}
		argv[k] = read_token(line, i, end, len, status, status_len, env);
		if (!argv[k++])
			return (free(status), free_argv(argv), NULL);
		i = skip_blanks(line, end);
	}
	argv[k] = NULL;
	free(status);
	i = 0;
	k = 0;
	while (argv[i])
	{
		if (argv[i][0] == '\0'
			&& (token_meta_flags(argv[i]) & TOKEN_META_QUOTED) == 0)
		{
			token_meta_forget(argv[i]);
			free(argv[i]);
		}
		else
			argv[k++] = argv[i];
		i++;
	}
	argv[k] = NULL;
	return (argv);
}
