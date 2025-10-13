/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 13:33:48 by mananton          #+#    #+#             */
/*   Updated: 2025/10/13 14:32:35 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_export_internal.h"

int	builtin_export(t_env *env, char **argv)
{
	size_t	i;
	int		status;

	if (!env || !argv)
		return (1);
	if (!argv[1])
		return (print_sorted_export(env));
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (export_process_argument(env, argv[i]))
			status = 1;
		i++;
	}
	return (status);
}
