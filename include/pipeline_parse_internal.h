/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parse_internal.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:32:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 16:32:12 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPELINE_PARSE_INTERNAL_H
# define PIPELINE_PARSE_INTERNAL_H

# include "minishell.h"

int	count_pipes(char **av);
int	split_fill(char **argv, char ***tmp, int count);

#endif
