/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 14:23:12 by cleticia          #+#    #+#             */
/*   Updated: 2022/06/02 12:18:53 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	se_interrupt_process(int signal)
{
	(void)signal;
	g_global.exit_status = 130;
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	se_quit_process(int signal)
{
	(void)signal;
	g_global.exit_status = 131;
	ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
}

void	se_set_exec_signals(void)
{
	signal(SIGINT, se_interrupt_process);
	signal(SIGQUIT, se_quit_process);
}

void	se_handler(int signal)
{
	if (signal == SIGINT)
	{
		g_global.exit_status = 130;
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	se_set_main_signals(void)
{
	signal(SIGINT, se_handler);
	signal(SIGQUIT, SIG_IGN);
}
