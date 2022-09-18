/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/25 16:41:17 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exit_digit_to_str(char *str)
{
	int	c;

	c = 0;
	while (str[c])
	{
		if (!(str[c] >= 48 && str[c] <= 57))
			return (0);
		c++;
	}
	return (1);
}

static void	exit_free(void)
{
	fl_free_var_list(&g_global.head_env);
	if (g_global.head_local)
		fl_free_var_list(&g_global.head_local);
	fl_free_list();
}

void	exit_message(void)
{
	exit_free();
	ft_putstr_fd("exit\n", STDERR_FILENO);
	exit(g_global.exit_status);
}

void	exit_status_one(void)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
	g_global.exit_status = 1;
	return ;
}

void	ft_exit(t_cmd *node, char **tokens)
{
	int	words;

	words = 0;
	while (tokens[words] != NULL)
		words++;
	if (node->pre_flag == 0 && words == 1)
	{
		exit_free();
		exit(g_global.exit_status);
	}
	else if (words == 2 && exit_digit_to_str(tokens[1]))
		g_global.exit_status = ft_atoi(tokens[1]);
	else if (words > 2 && exit_digit_to_str(tokens[1]))
		exit_status_one();
	else
	{
		ft_putstr_fd("exit: numeric argument required\n", STDERR_FILENO);
		exit_free();
		g_global.exit_status = 2;
		exit(g_global.exit_status);
	}
	exit_message();
}
