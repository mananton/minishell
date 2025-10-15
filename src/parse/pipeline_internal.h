/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:32:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:30:08 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_INTERNAL_H
# define PIPELINE_INTERNAL_H

# include "parse/pipeline.h"

int	count_pipes(char **av);
int	split_fill(char **argv, char ***tmp, int count);

#endif
