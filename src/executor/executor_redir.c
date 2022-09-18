/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 21:24:04 by cleticia          #+#    #+#             */
/*   Updated: 2022/06/02 12:21:28 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	er_do_cmd(t_cmd *cur_cmd, char **cmd, t_redir *redir, char **env)
{
	char		*path;
	t_variables	*env_var;
	int			ret;

	ret = 0;
	env_var = NULL;
	path = NULL;
	env_var = el_search_var(g_global.head_env, "PATH");
	if (env_var)
		path = eu_find_cmdpath(env_var->value, redir->commands[0]);
	if (ft_no_path_cmd(path, cmd[0]))
		return (127);
	else if (ec_is_builtin(cmd))
		ret = ec_exec_builtin(cur_cmd, cmd);
	else
		ret = execve(path, cmd, env);
	if (ret == -1)
	{
		ft_error_cmd_not_found(cmd[0]);
		return (127);
	}
	free(path);
	return (g_global.exit_status);
}

int	er_rednpipe(t_cmd *cmd, t_redir *redir, int *saved_std, char **env)
{
	int	ret;

	ret = 0;
	if (redir->has_heredoc != -1)
	{
		ret = eh_exec_here(cmd, redir, saved_std[1], env);
		if (ret == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (eu_is_here_interrupted(ret, redir, env))
			return (ret);
	}
	else
		ret = er_no_pipesheredoc(redir, cmd, env);
	return (ret);
}

int	er_redpiped(t_cmd *cmd, t_redir *redir, int *saved_std, char **env)
{
	int	ret;

	ret = 0;
	if (redir->has_out != -1 || cmd->type_cmd == 1)
		ret = er_prepare_stdout(cmd, redir);
	if (ret == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (redir->has_heredoc != -1)
	{
		ret = eh_herepipe(cmd, redir, saved_std[1], env);
		if (ret == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (eu_is_here_interrupted(ret, redir, env))
			return (ret);
	}
	else if ((redir->has_in != -1 || cmd->pre_flag == 1)
		&& !redir->filename_failed)
		ret = er_prepare_stdin(cmd, redir);
	if (ret == EXIT_FAILURE || redir->filename_failed)
		return (EXIT_FAILURE);
	if (ec_is_builtin(cmd->tokens))
		ret = ec_exec_builtin(cmd, redir->commands);
	else
		ret = er_redir_child(cmd, redir, env);
	return (ret);
}

void	er_treat_redir_no_cmd(t_cmd *cur_cmd)
{
	if (pr_is_token_redir(cur_cmd->tokens[0]) && cur_cmd->type_cmd == 1)
	{
		cur_cmd->next->pre_flag = 1;
		pipe(cur_cmd->next->fd);
		close(cur_cmd->next->fd[1]);
	}
	if (pr_is_token_redir(cur_cmd->tokens[0]) && cur_cmd->pre_flag == 1)
	{
		if (cur_cmd->fd[0] != 0)
			close(cur_cmd->fd[0]);
	}
}

int	er_execute_redires(t_cmd *cur_cmd, int *saved_stds)
{
	int		ret;
	t_redir	*redir;
	char	**envs;

	ret = 0;
	envs = el_env_to_matrix();
	redir = sr_treat_redir(cur_cmd, &ret);
	if (ret == 1 || pr_is_token_redir(cur_cmd->tokens[0])
		|| ft_strchr(cur_cmd->tokens[0], '='))
	{
		er_exec_no_command(cur_cmd, redir, envs);
		return (ret);
	}
	if (!eu_is_cmd_piped(cur_cmd))
		ret = er_rednpipe(cur_cmd, redir, saved_stds, envs);
	else if (eu_is_cmd_piped(cur_cmd))
		ret = er_redpiped(cur_cmd, redir, saved_stds, envs);
	ft_unlink_heredoc(redir);
	if (ret == 130 || ret == 2)
	{
		ft_signal_interrupt(&ret, redir, saved_stds);
		return (ret);
	}
	er_free_final_redir(redir, saved_stds, envs);
	return (ret);
}
