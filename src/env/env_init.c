/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:38:04 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 13:54:23 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_init(char **envp)
{
	t_env *env;   /* ponteiro para a nossa estrutura t_env           */
	size_t i;     /* índice para percorrer envp                        */
	size_t count; /* quantas entradas existem em envp                  */
	
	/* 1) aloca a struct t_env (guarda o vetor de "KEY=VALUE") */
	
	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL); /* sem memória → aborta */
		
	/* 2) conta quantas strings existem em envp (até encontrar NULL) */
	
	count = 0;
	while (envp && envp[count])
		count++;
		
	/* 3) aloca o vetor de ponteiros para char*: tamanho = count + 1 */
	/*    (+1 para guardar o terminador NULL no final do vetor)       */
	
	env->vars = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env->vars)
		return (free(env), NULL); /* libera struct e aborta em erro */
	
	/* 4) duplica cada linha "KEY=VALUE" de envp para o nosso vetor   */
	/*    usamos ft_strdup para ter uma cópia própria (que é nossa)   */
	
	i = 0;
	while (envp && envp[i])
	{
		env->vars[i] = ft_strdup(envp[i]);
		if (!env->vars[i])
			return (env_free(env), NULL); /* limpa tudo e aborta */
		i++;
	}
	
	/* 5) coloca NULL para marcar o fim do vetor */
	
	env->vars[i] = NULL;
	
	env->last_status = 0;  /* primeiro valor padrão */
	
	/* 6) devolve a estrutura pronta */
	
	return (env);
}
