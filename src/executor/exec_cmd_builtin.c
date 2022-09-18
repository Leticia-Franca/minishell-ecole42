/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:38:53 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 19:57:03 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ec_exec_builtin(t_cmd *current_cmd, char **cmd)
{
	char	*builtin;
	int		len;

	builtin = current_cmd->tokens[0];
	len = ft_strlen(builtin);
	if (!ft_strncmp(builtin, "cd", len))
		ft_cd(cmd);
	else if (!ft_strncmp(builtin, "echo", len))
		ft_echo(cmd);
	else if (!ft_strncmp(builtin, "env", len))
		ft_env(cmd);
	else if (!ft_strncmp(builtin, "exit", len))
		ft_exit(current_cmd, cmd);
	else if (!ft_strncmp(builtin, "export", len))
		ft_export(cmd);
	else if (!ft_strncmp(builtin, "pwd", len))
		ft_pwd();
	else if (!ft_strncmp(builtin, "unset", len))
		ft_unset(cmd);
	else
		return (1);
	return (g_global.exit_status);
}

int	ec_is_builtin(char **tokens)
{
	int	len;

	len = ft_strlen(tokens[0]);
	if (!ft_strncmp(tokens[0], "cd", len)
		|| !ft_strncmp(tokens[0], "echo", len)
		|| !ft_strncmp(tokens[0], "env", len)
		|| !ft_strncmp(tokens[0], "exit", len)
		|| !ft_strncmp(tokens[0], "export", len)
		|| !ft_strncmp(tokens[0], "pwd", len)
		|| !ft_strncmp(tokens[0], "unset", len))
		return (1);
	return (0);
}
