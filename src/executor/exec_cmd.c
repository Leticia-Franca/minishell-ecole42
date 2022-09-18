/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/27 17:52:07 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	ec_exec_fork(t_cmd *cmd, t_cmd *nextcd, char **env, char *path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	se_set_exec_signals();
	if (pid == 0)
	{
		g_global.exit_status = ep_exec_child(cmd, nextcd, env, path);
		exit(g_global.exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_global.exit_status = WEXITSTATUS(status);
	if (path)
		free(path);
	if (cmd->type_cmd == 1)
		close(nextcd->fd[1]);
	if (cmd->fd[0] != 0)
		close(cmd->fd[0]);
}

int	ec_exec_pipe(t_cmd *current_cmd, char **env_matrix)
{
	t_cmd		*next_cmd;
	t_variables	*env_var;
	char		*path;

	path = NULL;
	env_var = NULL;
	next_cmd = current_cmd->next;
	eu_open_pipe(current_cmd, next_cmd);
	if (!ft_strchr(current_cmd->tokens[0], '='))
	{
		env_var = el_search_var(g_global.head_env, "PATH");
		if (env_var)
			path = eu_find_cmdpath(env_var->value, current_cmd->tokens[0]);
		if (!path)
		{
			if (current_cmd->type_cmd == 1)
				close(next_cmd->fd[1]);
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(current_cmd->tokens[0], 2);
			ft_putendl_fd(": command not found", 2);
			return (127);
		}
	}
	ec_exec_fork(current_cmd, next_cmd, env_matrix, path);
	return (g_global.exit_status);
}

int	ec_no_redir_exec(t_cmd *cur_cmd, int *saved_stds)
{
	char	**env_matrix;
	int		ret;

	ret = 0;
	env_matrix = el_env_to_matrix();
	if (ec_is_builtin(cur_cmd->tokens) && !eu_is_cmd_piped(cur_cmd))
	{
		ef_treat_exit(cur_cmd, saved_stds, env_matrix);
		ret = ec_exec_builtin(cur_cmd, cur_cmd->tokens);
	}
	else
		ret = ec_exec_pipe(cur_cmd, env_matrix);
	ef_free_envs(env_matrix);
	return (ret);
}

static int	ec_is_to_continue(t_cmd **current_cmd, int ret)
{
	if (ret == 1 || pr_is_token_redir((*current_cmd)->tokens[0])
		|| ret == 132)
	{
		(*current_cmd) = (*current_cmd)->next;
		return (1);
	}
	return (0);
}

int	ec_exec_process(t_cmd *head, int *saved_stds)
{
	t_cmd	*current_cmd;
	int		ret;

	current_cmd = head;
	ret = 0;
	while (current_cmd)
	{
		if (ev_is_valid_declare(current_cmd))
			ret = ev_declare_var(current_cmd->tokens);
		if (current_cmd->has_redir == 1)
		{
			ret = er_execute_redires(current_cmd, saved_stds);
			if (ret == 130 || ret == 2)
				break ;
			else if (ec_is_to_continue(&current_cmd, ret))
				continue ;
		}
		else if (!ev_is_valid_declare(current_cmd))
			ret = ec_no_redir_exec(current_cmd, saved_stds);
		current_cmd = current_cmd->next;
	}
	ef_finalize_exec_process(ret, saved_stds);
	return (ret);
}
