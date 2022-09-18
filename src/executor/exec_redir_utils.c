/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 09:12:39 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/27 18:05:51 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_print_error_filename(char *filename)
{
	ft_putstr_fd("bash: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putendl_fd(": Inexistent file or directory", STDERR_FILENO);
	free(filename);
}

void	ft_error_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
}

int	ft_no_path_cmd(char *path, char *cmd)
{
	if (!path)
	{
		ft_error_cmd_not_found(cmd);
		return (1);
	}
	return (0);
}

void	ft_unlink_heredoc(t_redir *redir)
{
	if (redir->has_heredoc != -1)
		unlink("./tmp_heredoc.txt");
}

void	ft_signal_interrupt(int	*ret, t_redir *redir, int *saved_stds)
{
	if ((*ret) == 130 && redir->has_heredoc == -1)
		(*ret) = 132;
	er_free_signal_interrupt(redir, saved_stds);
}
