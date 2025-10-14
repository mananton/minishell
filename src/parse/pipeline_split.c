/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:32:58 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 16:32:58 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline_parse_internal.h"
#include <stdlib.h>

int	split_pipeline(char **argv, char ****segs, int *count)
{
	char	***tmp;

	*segs = NULL;
	*count = 0;
	if (!argv || !argv[0])
		return (1);
	*count = count_pipes(argv) + 1;
	tmp = (char ***)malloc(sizeof(char **) * (*count));
	if (!tmp)
		return (1);
	if (split_fill(argv, tmp, *count) != 0)
	{
		free(tmp);
		return (1);
	}
	*segs = tmp;
	return (0);
}
