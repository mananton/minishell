/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:47:01 by mananton          #+#    #+#             */
/*   Updated: 2025/09/29 13:54:24 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/main.c */
#include "minishell.h"
#include <readline/readline.h>   // readline() → lê uma linha com prompt
#include <readline/history.h>    // add_history() → guarda no histórico

/*
** Função principal do minishell.
** - Cria uma cópia do ambiente (env).
** - Corrige o SHLVL.
** - Entra num loop infinito para ler comandos.
** - Executa builtins simples ou avisa "command not found".
*/
int	main(int argc, char **argv_unused, char **envp)
{
	t_env	*env;     // ponteiro para a nossa tabela de variáveis de ambiente
	char	*line;    // string com a linha digitada pelo usuário
	char	**argv;   // vetor de strings (palavras da linha → comando + args)
	int		status;  // código de retorno do builtin executado

	(void)argc;           // não usamos argc → evitamos warning
	(void)argv_unused;    // não usamos argv → idem

	// Inicializa nossa cópia do ambiente a partir do envp herdado do pai
	env = env_init(envp);
	if (!env)
		// Se der erro na alocação, avisa e termina o programa com status 1
		return (put_str_fd("minishell: env init failed\n", 2), 1);

	// Corrige a variável SHLVL (incrementa +1, normaliza valores inválidos)
	(void)env_fix_shlvl(env);

	// Loop principal do shell
	while (1)
	{
		// Mostra o prompt "minishell$ " e espera input do usuário
		line = readline("minishell$ ");
		if (!line)
			break;  // se readline devolve NULL (Ctrl+D) → sai do loop

		// Se a linha não for vazia, guarda no histórico
		if (*line)
			add_history(line);

		// Divide a linha em palavras (muito simples, separado só por espaço/tab)
		argv = split_simple_args(line);

		// Se temos comando (argv[0]) e ele é builtin
		if (argv && argv[0] && is_builtin(argv[0]))
		{
			// Executa o builtin e guarda retorno
			status = run_builtin(argv, env);

			// Se o builtin foi "exit", sinaliza para encerrar shell
			if (status == MS_BUILTIN_EXIT)
			{
				free_argv(argv);  // libera o vetor de argumentos
				free(line);       // libera a linha lida
				break;            // sai do loop principal
			}
		}
		// Caso não seja builtin mas ainda tenha algo digitado
		else if (argv && argv[0] && *argv[0])
		{
			// Mostra erro padrão de "command not found"
			put_str_fd("minishell: command not found: ", 2);
			put_str_fd(argv[0], 2);
			put_str_fd("\n", 2);
		}

		// Libera memória usada nesta iteração
		free_argv(argv);
		free(line);
	}

	// Saiu do loop (Ctrl+D ou "exit") → libera env e termina o programa
	env_free(env);
	return (0);
}

