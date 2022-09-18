/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 20:28:54 by cleticia          #+#    #+#             */
/*   Updated: 2022/06/02 12:19:33 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	eh_herepipe(t_cmd *cmd, t_redir *redir, int saved_out, char **env)
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
	if (cmd->fd[0] != 0)
		close(cmd->fd[0]);
	if (redir->has_in != -1 && redir->filename_failed)
	{
		close(tmp_fd);
		return (EXIT_FAILURE);
	}
	ret = er_prep_stdin_redir(cmd, redir, tmp_fd);
	if (ret == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
