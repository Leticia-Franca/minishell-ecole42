/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 17:26:38 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/25 15:11:19 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ep_builtin_pipe(t_cmd *current_cmd, char **env_matrix, char *path)
{
	if (path)
		free(path);
	ec_exec_builtin(current_cmd, current_cmd->tokens);
	er_free_matrix(env_matrix);
	fl_free_list();
	fl_free_var_list(&g_global.head_env);
	if (g_global.head_local)
		fl_free_var_list(&g_global.head_local);
	free(env_matrix);
	return (g_global.exit_status);
}

void	ep_set_fd_pipe(t_cmd *current_cmd, t_cmd *next_cmd)
{
	if (current_cmd->type_cmd == 1)
	{
		dup2(next_cmd->fd[1], STDOUT_FILENO);
		close(next_cmd->fd[1]);
	}
	if (current_cmd->fd[0] != 0)
	{
		dup2(current_cmd->fd[0], STDIN_FILENO);
		close(current_cmd->fd[0]);
	}		
}

int	ep_exec_child(t_cmd *cmd, t_cmd *next_cmd, char **env, char *path)
{
	int	ret;

	ret = 0;
	ep_set_fd_pipe(cmd, next_cmd);
	if (ft_strchr(cmd->tokens[0], '=') != 0)
	{
		er_free_matrix(env);
		fl_free_list();
		fl_free_var_list(&g_global.head_env);
		if (g_global.head_local)
			fl_free_var_list(&g_global.head_local);
		free(env);
		return (ret);
	}
	else if (ec_is_builtin(cmd->tokens))
		ret = ep_builtin_pipe(cmd, env, path);
	else
		ret = execve(path, cmd->tokens, env);
	if (ret == -1)
	{
		ft_putstr_fd(cmd->tokens[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	return (ret);
}
