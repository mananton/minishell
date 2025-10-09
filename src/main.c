/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:47:01 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 11:09:39 by mananton         ###   ########.fr       */
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
** - Executa builtins simples, comandos externos e redireções.
*/
int	main(int argc, char **argv_unused, char **envp)
{
	t_env	*env;        // tabela de variáveis de ambiente
	char	*line;       // linha lida do utilizador
	char	**argv;      // vetor original de argumentos
	char	**argv_clean;// vetor sem tokens de redireção
	int		status;      // código de retorno da execução
	t_redir	r;           // estrutura com ficheiros de redireção
	int		saved_in;    // stdin original (para restaurar)
	int		saved_out;   // stdout original (para restaurar)
	int		fd_in;       // descritor de ficheiro de entrada
	int		fd_out;      // descritor de ficheiro de saída

	(void)argc;
	(void)argv_unused;
	// Inicializa o ambiente herdado e valida
	env = env_init(envp);
	if (!env)
		return (put_str_fd("minishell: env init failed\n", 2), 1);
	(void)env_fix_shlvl(env); // ajusta SHLVL

	// Loop principal: lê e executa comandos
	while (1)
	{
		line = readline("minishell$ ");     // mostra prompt e lê input
		if (!line)
			break ;                        // Ctrl+D → termina o shell
		if (*line)
			add_history(line);             // guarda no histórico
		argv = split_args_quotes(line);    // divide a linha em tokens

		// Separa redireções (>, >>, <) e devolve argv “limpo”
		if (parse_redirs(argv, &argv_clean, &r) != 0)
		{
			free_argv(argv);
			free(argv_clean);
			free(line);
			continue;                      // erro de sintaxe → ignora linha
		}

		// Se for builtin
		if (argv_clean && argv_clean[0] && is_builtin(argv_clean[0]))
		{
			// aplica redireções temporárias no pai
			if (apply_redirs_parent(&r, &saved_in, &saved_out, &fd_in, &fd_out) != 0)
				status = 1;
			else
			{
				status = run_builtin(argv_clean, env);
				restore_stdio_parent(saved_in, saved_out, fd_in, fd_out);
			}
			// verifica se é exit
			if (status >= MS_BUILTIN_EXIT)
			{
				int exit_code = status - MS_BUILTIN_EXIT;
				free_argv(argv);
				free(argv_clean);
				free(line);
				env_free(env);
				return (exit_code & 0xFF);
			}
			env->last_status = status & 0xFF;
		}
		// Caso contrário → comando externo
		else if (argv_clean && argv_clean[0] && *argv_clean[0])
		{
			status = exec_external_redir(argv_clean, env, &r);
			env->last_status = status & 0xFF;
		}

		// Liberta memória da iteração
		free_argv(argv);
		free(argv_clean);
		free(line);
	}
	env_free(env);
	return (0);
}
