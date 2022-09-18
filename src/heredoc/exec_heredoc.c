/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:56:09 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	er_prep_stdin_redir(t_cmd *cmd, t_redir *redir, int tmp_fd)
{
	int	ret;

	if (redir->has_in != -1 && redir->has_in > redir->has_heredoc)
	{
		close(tmp_fd);
		ret = er_prepare_stdin(cmd, redir);
		if (ret == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		dup2(tmp_fd, STDIN_FILENO);
		close(tmp_fd);
	}
	return (EXIT_SUCCESS);
}

void	eh_interrupt_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		g_global.exit_status = 130;
		if (g_global.delimiter)
			free(g_global.delimiter);
		write (1, "\n", 1);
		exit(130);
	}
}

static int	eh_exec_heredoc_cmd(t_cmd *current_cmd, t_redir *redir, char **envs)
{
	int	pid;
	int	status;
	int	ret;

	ret = 0;
	pid = fork();
	se_set_exec_signals();
	if (pid == 0)
	{
		ret = er_do_cmd(current_cmd, redir->commands, redir, envs);
		eh_free_child_heredoc(redir, envs);
		rl_clear_history();
		if (ret == 0)
			exit(EXIT_SUCCESS);
		else
			exit(127);
	}
	wait(&status);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	return (ret);
}

static int	eh_stdout_filecheck(t_cmd *cmd, t_redir *redir, int tmp_fd)
{
	if (redir->has_out != -1)
		er_prepare_stdout(cmd, redir);
	if (redir->has_in != -1 && redir->filename_failed)
	{
		close(tmp_fd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	eh_exec_here(t_cmd *cmd, t_redir *redir, int saved_out, char **env)
{
	int		tmp_fd;
	int		ret;
	int		save_out;

	tmp_fd = 0;
	ret = 0;
	save_out = dup(1);
	dup2(saved_out, STDOUT_FILENO);
	signal(SIGINT, SIG_IGN);
	ret = eh_read_heredoc(redir, env, &tmp_fd);
	if (ret != EXIT_SUCCESS)
		return (ret);
	dup2(save_out, STDOUT_FILENO);
	close(save_out);
	if (eh_stdout_filecheck(cmd, redir, tmp_fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ret = er_prep_stdin_redir(cmd, redir, tmp_fd);
	if (ret == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (ec_is_builtin(cmd->tokens))
		ec_exec_builtin(cmd, redir->commands);
	else
		g_global.exit_status = eh_exec_heredoc_cmd(cmd, redir, env);
	return (g_global.exit_status);
}
