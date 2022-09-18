/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:44:24 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	env_print_list(t_variables *env_tmp)
{
	ft_putstr_fd(env_tmp->name, STDOUT_FILENO);
	ft_putstr_fd("=", STDOUT_FILENO);
	ft_putendl_fd(env_tmp->value, STDOUT_FILENO);
}

static int	env_print_error(char **cmd)
{
	int	index;

	index = 1;
	while (cmd[index])
	{
		if (!ft_strchr(cmd[index], '='))
		{
			ft_putstr_fd("env: ", STDERR_FILENO);
			ft_putstr_fd("\"", STDERR_FILENO);
			ft_putstr_fd(cmd[index], STDERR_FILENO);
			ft_putstr_fd("\": ", STDERR_FILENO);
			ft_putendl_fd("Inexistent file or directory", STDERR_FILENO);
			g_global.exit_status = 127;
			return (1);
		}
		index++;
	}
	return (0);
}

static void	env_arg(char **cmd)
{
	int	index;

	index = 0;
	while (cmd[++index])
	{
		if (ft_strchr(cmd[index], '='))
			ft_putstr_fd(cmd[index], STDOUT_FILENO);
		if (cmd[index + 1] != NULL)
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	ft_env(char **cmd)
{
	t_variables	*env_tmp;

	if (cmd[1] != NULL)
	{
		if (env_print_error(cmd) == 1)
			return ;
	}
	env_tmp = g_global.head_env;
	while (env_tmp->next != NULL)
	{
		env_print_list(env_tmp);
		env_tmp = env_tmp->next;
	}
	if (!env_tmp->next)
		env_print_list(env_tmp);
	if (cmd[1] != NULL)
		env_arg(cmd);
	g_global.exit_status = 0;
}
