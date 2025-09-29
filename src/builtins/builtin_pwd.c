/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:56:43 by mananton          #+#    #+#             */
/*   Updated: 2025/09/25 09:57:36 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// retorna 0 em sucesso, !=0 em erro (convenção simples por enquanto)
int	builtin_pwd(void)
{
	char *cwd = getcwd(NULL, 0);
		// pede ao sistema para alocar a string do diretório atual
	if (!cwd)
	{
		put_str_fd("minishell: pwd: error getting cwd\n", 2);
		return (1);
	}
	put_str_fd(cwd, 1);
	put_str_fd("\n", 1);
	free(cwd);
	return (0);
}
