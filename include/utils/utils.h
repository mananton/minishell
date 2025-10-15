/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:46:28 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:37:47 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

void						put_str_fd(const char *s, int fd);
char						**split_simple_args(const char *line);
void						free_argv(char **argv);
char						*ft_itoa(int n);
int							ft_atoi_strict(const char *s, int *ok);
char						*ft_strdup(const char *s);
int							is_valid_ident(const char *s);

#endif
