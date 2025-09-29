/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:47:01 by mananton          #+#    #+#             */
/*   Updated: 2025/09/26 12:00:24 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

/*
** Versão didática do main:
** - Recebe envp (a "foto" do ambiente do sistema)
** - Cria env (nossa "mochila" viva) com env_init(envp)
** - Loop: lê linha, quebra em argv, executa builtin se for o caso
** - Ao terminar, libera a mochila com env_free
*/
int	main(int argc, char **argv_unused, char **envp)
{
	t_env	*env;   /* nossa cópia do ambiente (tabela KEY=VALUE)        */
	char	*line;  /* linha digitada pelo usuário (readline aloca)       */
	char	**argv; /* vetor de palavras separadas (comando + argumentos) */

	(void)argc;       /* não usamos argc/argv aqui                          */
	(void)argv_unused;

	/* cria a mochila a partir da foto inicial (envp) */
	env = env_init(envp);
	if (!env)
		return (put_str_fd("minishell: env init failed\n", 2), 1);

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)   /* Ctrl-D → readline retorna NULL → sair do shell */
			break ;
		if (*line)
			add_history(line); /* guarda no histórico se a linha não for vazia */

		/* separa por espaços/tabs (versão simples, sem aspas ainda) */
		argv = split_simple_args(line);

		/* se for builtin (pwd, echo, cd...), executa passando a mochila env */
		if (argv && argv[0] && is_builtin(argv[0]))
			(void)run_builtin(argv, env);
		else if (argv && argv[0] && *argv[0])
		{
			/* ainda não executamos externos → mensagem padrão */
			put_str_fd("minishell: command not found: ", 2);
			put_str_fd(argv[0], 2);
			put_str_fd("\n", 2);
		}

		free_argv(argv); /* libera o vetor argv (e cada string dentro) */
		free(line);      /* libera a linha alocada pelo readline       */
	}

	/* ao sair do shell, libera toda a mochila do ambiente */
	env_free(env);
	return (0);
}

