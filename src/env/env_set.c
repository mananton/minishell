/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 11:26:26 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 11:26:31 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** join_kv:
** Constrói a string "KEY=VALUE" nova, alocada na heap.
** Passos:
** - valida key e value (não podem ser NULL; key não pode ser vazia);
** - calcula tamanhos de key e value;
** - aloca klen + 1 ('=') + vlen + 1 ('\0');
** - copia key, coloca '=', copia value, fecha com '\0';
** - retorna o ponteiro da nova string (quem chama deve dar free).
*/
static char	*join_kv(const char *key, const char *value)
{
	size_t	klen;
	size_t	vlen;
	char	*e;

	if (!key || !*key || !value)
		return (NULL);
	klen = strlen(key);
	vlen = strlen(value);
	e = (char *)malloc(klen + 1 + vlen + 1);
	if (!e)
		return (NULL);
	memcpy(e, key, klen);
	e[klen] = '=';
	memcpy(e + klen + 1, value, vlen);
	e[klen + 1 + vlen] = '\0';
	return (e);
}

/*
** env_set:
** Cria/atualiza uma entrada "KEY=VALUE" no vetor env->vars.
** Fluxo:
** 1) valida env/key/value;
** 2) monta "KEY=VALUE" com join_kv;
** 3) procura se a KEY já existe (env_find_index);
**    - se existir: libera a antiga e coloca a nova no mesmo índice;
**    - se não existir: cresce o vetor (env_grow) e insere no fim;
** 4) garante o terminador NULL depois da última entrada;
** 5) retorna 0 em sucesso, 1 em erro.
*/
int	env_set(t_env *env, const char *key, const char *value)
{
	char	*entry; /* nova "KEY=VALUE" a inserir/substituir */
	int		idx;   /* índice da KEY se já existir; -1 caso não exista */
	size_t	i;      /* índice para achar o fim do vetor quando for inserir */

	if (!env || !key || !*key || !value)
		return (1);
	entry = join_kv(key, value);
	if (!entry)
		return (1);
	idx = env_find_index(env, key);
	if (idx >= 0)
	{
		free(env->vars[idx]);   /* substitui a entrada antiga */
		env->vars[idx] = entry;
		export_mark_remove(key);
		return (0);
	}
	if (!env_grow(env, 1))     /* precisa de espaço para mais 1 entrada */
		return (free(entry), 1);
	i = 0;
	while (env->vars[i])       /* encontra o NULL atual (fim) */
		i++;
	env->vars[i] = entry;      /* coloca a nova entrada no fim */
	env->vars[i + 1] = NULL;   /* mantém o vetor terminado por NULL */
	export_mark_remove(key);
	return (0);
}
