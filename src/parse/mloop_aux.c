/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mloop_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/24 22:40:12 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ma_decide_exec(int ret)
{
	int		saved_stds[2];

	if (ret == 0)
	{
		saved_stds[1] = dup(STDOUT_FILENO);
		saved_stds[0] = dup(STDIN_FILENO);
		ec_exec_process(g_global.head, saved_stds);
	}
	else
	{
		if (g_global.head != NULL)
			fl_free_list();
	}
}

void	ma_clear_ctrld(void)
{
	if (g_global.head)
		fl_free_list();
	if (g_global.head_env)
		fl_free_var_list(&g_global.head_env);
	if (g_global.head_local)
		fl_free_var_list(&g_global.head_local);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
