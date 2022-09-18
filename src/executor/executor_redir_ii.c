/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_ii.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 21:33:33 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/24 22:38:20 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	er_redir_child(t_cmd *cmd, t_redir *redir, char **env)
{
	int	pid;
	int	status;

	pid = fork();
	se_set_exec_signals();
	if (pid == 0)
	{
		g_global.exit_status = er_do_cmd(cmd, redir->commands, redir, env);
		fl_free_list();
		fl_free_var_list(&g_global.head_env);
		if (g_global.head_local)
			fl_free_var_list(&g_global.head_local);
		ef_free_redir_envs(redir, env);
		free(redir);
		exit(g_global.exit_status);
	}
	wait(&status);
	if (WIFEXITED(status))
		g_global.exit_status = WEXITSTATUS(status);
	if (g_global.exit_status == 130)
	{
		ef_free_redir(redir);
		ef_free_envs(env);
	}
	return (g_global.exit_status);
}

int	er_no_pipesheredoc(t_redir *redir, t_cmd *cmd, char **env)
{
	int	ret;

	ret = 0;
	if (redir->has_out != -1)
		ret = er_prepare_stdout(cmd, redir);
	if (ret == EXIT_FAILURE)
		return (ret);
	if (redir->has_in != -1)
		ret = er_prepare_stdin(cmd, redir);
	if (ret == EXIT_FAILURE || redir->filename_failed)
		return (EXIT_FAILURE);
	if (ec_is_builtin(cmd->tokens))
		ret = ec_exec_builtin(cmd, redir->commands);
	else
		ret = er_redir_child(cmd, redir, env);
	return (ret);
}

void	er_exec_no_command(t_cmd *cmd, t_redir *redir, char **env)
{
	if (eu_is_cmd_piped(cmd))
		er_treat_redir_no_cmd(cmd);
	ef_free_redir_envs(redir, env);
	free(redir);
}

void	er_free_signal_interrupt(t_redir *redir, int *saved_stds)
{
	dc_dup_stds(saved_stds);
	if (redir->filename_failed)
		free(redir->filename_failed);
	free(redir);
}

void	er_free_final_redir(t_redir *redir, int *saved_stds, char **env)
{
	if (redir->filename_failed)
		ft_print_error_filename(redir->filename_failed);
	dc_dup_stds(saved_stds);
	ef_free_redir(redir);
	free(redir);
	ef_free_envs(env);
}
