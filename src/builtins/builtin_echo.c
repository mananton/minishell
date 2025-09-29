/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 10:12:00 by mananton          #+#    #+#             */
/*   Updated: 2025/09/25 10:49:37 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Função auxiliar: verifica se a string é uma opção -n válida.
 * Regras aceitas aqui:
 *   "-n", "-nn", "-nnnn" ... (qualquer quantidade de 'n' após o '-')
 * Retorna 1 se for opção -n válida; 0 caso contrário.
 */
static int	is_valid_n_option(const char *s)
{
	size_t	i;

	if (!s || s[0] != '-')
		return (0);
	if (s[1] == '\0')
		return (0); // apenas "-" não é opção válida
	i = 1;
	while (s[i] == 'n')
		i++;
	/* só é válido se TODOS os caracteres após '-' forem 'n' */
	return (s[i] == '\0');
}

/*
 * builtin_echo (com -n):
 * - Sem argumentos: imprime "\n"
 * - Com argumentos sem -n: imprime args separados por espaço e depois "\n"
 * - Com uma ou mais opções -n (ex.: "-n", "-nnn", "-n
	-n"): imprime args sem "\n"
 * - Paramos de tratar opções quando encontrar algo que não seja "-n..." puro.
 */
int	builtin_echo(char **argv)
{
	size_t	i;
	int		no_newline;

	/* argv[0] = "echo"; começamos a olhar a partir de argv[1] */
	i = 1;
	no_newline = 0;
	/* 1) Varre opções -n no início */
	while (argv && argv[i] && is_valid_n_option(argv[i]))
	{
		no_newline = 1;
		i++;
	}
	/* 2) Imprime o restante dos argumentos separados por espaço */
	while (argv && argv[i])
	{
		put_str_fd(argv[i], 1);
		if (argv[i + 1])
			put_str_fd(" ", 1);
		i++;
	}
	/* 3) Se não houver -n, imprime quebra de linha ao final */
	if (!no_newline)
		put_str_fd("\n", 1);
	return (0);
}
