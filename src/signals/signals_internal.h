/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:40:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 12:32:28 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_INTERNAL_H
# define SIGNALS_INTERNAL_H

# include <signal.h>

struct	s_env;

extern volatile sig_atomic_t	g_signal_last;
void	signals_update_env(struct s_env *env);

#endif
