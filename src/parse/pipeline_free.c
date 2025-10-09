/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 13:59:58 by mananton          #+#    #+#             */
/*   Updated: 2025/10/09 14:00:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/* free dos vetores de segmentos (N vetores) */
void	free_segments(char ***segs, int count)
{
	int i;

	if (!segs || !*segs)
		return ;
	i = 0;
	while (i < count)
	{
		free((*segs)[i]);
		i++;
	}
	free(*segs);
	*segs = NULL;
}
