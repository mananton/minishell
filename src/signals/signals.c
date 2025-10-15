/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 09:42:11 by mananton          #+#    #+#             */
/*   Updated: 2025/10/11 09:42:11 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals_internal.h"
#include "signals/signals.h"
#include "env/env.h"
#include <unistd.h>
#include <readline/readline.h>

volatile sig_atomic_t	g_signal_last = 0;

static void	prompt_sigint(int sig)
{
	g_signal_last = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signals_setup_interactive(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = prompt_sigint;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	g_signal_last = 0;
}

void	signals_setup_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

void	signals_update_env(t_env *env)
{
	sig_atomic_t	sig;

	if (!env)
		return ;
	sig = g_signal_last;
	if (sig == 0)
		return ;
	g_signal_last = 0;
	if (sig == SIGINT)
		env->last_status = 130;
}
