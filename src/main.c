/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:47:01 by mananton          #+#    #+#             */
/*   Updated: 2025/10/06 14:05:07 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* src/main.c */
#include "minishell.h"
#include <readline/history.h>  // add_history() → guarda no histórico
#include <readline/readline.h> // readline() → lê uma linha com prompt

/*
** Função principal do minishell.
** - Cria uma cópia do ambiente (env).
** - Corrige o SHLVL.
** - Entra num loop infinito para ler comandos.
** - Executa builtins simples ou avisa "command not found".
*/
int	main(int argc, char **argv_unused, char **envp)
{
	t_env *env;  // ponteiro para a nossa tabela de variáveis de ambiente
	char *line;  // string com a linha digitada pelo usuário
	char **argv; // vetor de strings (palavras da linha → comando + args)
	int status;  // código de retorno do builtin executado
	(void)argc;        // não usamos argc → evitamos warning
	(void)argv_unused; // não usamos argv → idem
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
			break ; // se readline devolve NULL (Ctrl+D) → sai do loop
		// Se a linha não for vazia, guarda no histórico
		if (*line)
			add_history(line);
		// Divide a linha em palavras (muito simples, separado só por espaço/tab)
		argv = split_args_quotes(line);
		// Se temos comando (argv[0]) e ele é builtin
		if (argv && argv[0] && is_builtin(argv[0])) /* se é builtin...       */
		{
			status = run_builtin(argv, env); /* executa o builtin     */
			if (status >= MS_BUILTIN_EXIT) /* sentinela detectada?  */
			{
				int exit_code = status - MS_BUILTIN_EXIT;
					/* extrai o exit code    */
				free_argv(argv);                         
					/* limpa argv desta linha*/
				free(line);                              
					/* limpa a linha lida    */
				env_free(env);                           
					/* limpa a nossa env     */
				return (exit_code & 0xFF);               
					/* encerra minishell     */
			}
			env->last_status = status & 0xFF;
		}
		// Caso não seja builtin mas ainda tenha algo digitado
		else if (argv && argv[0] && *argv[0])
		{
			status = exec_external(argv, env);
			env->last_status = status & 0xFF;
		}

		// Libera memória usada nesta iteração
		free_argv(argv);
		free(line);
	}
	// Saiu do loop (Ctrl+D ou "exit") → libera env e termina o programa
	env_free(env);
	return (0);
}