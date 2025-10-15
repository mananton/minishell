/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 13:50:31 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:37:03 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "env/env.h"

void						signals_setup_interactive(void);
void						signals_setup_child(void);
void						signals_update_env(t_env *env);

#endif
