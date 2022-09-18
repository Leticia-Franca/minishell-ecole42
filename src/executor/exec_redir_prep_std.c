/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_prep_std.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/25 16:15:22 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	er_open_file_out(t_redir *redir)
{
	char	*redir_type;
	char	*file_name;
	int		fd;
	int		len;

	fd = 0;
	redir_type = redir->type[redir->has_out];
	file_name = redir->argv[redir->has_out + 1];
	len = ft_strlen(redir_type);
	if (redir->has_out != -1 && !ft_strncmp(redir_type, "DB_REDIR", len))
		fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0744);
	else if (redir->has_out != -1)
		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0744);
	return (fd);
}

static void	er_piping_next_cmd(t_cmd *next_cmd, t_redir *redir)
{
	next_cmd->pre_flag = 1;
	pipe(next_cmd->fd);
	if (redir->has_out != -1 || (redir->has_out == -1
			&& redir->filename_failed != NULL))
		close(next_cmd->fd[1]);
}

static void	er_stdout_to_file(int fd)
{
	dup2(fd, STDOUT_FILENO);
	close(fd);
}	

int	er_prepare_stdout(t_cmd *current_cmd, t_redir *redir)
{
	int		fd;
	t_cmd	*next_cmd;

	next_cmd = current_cmd->next;
	fd = 0;
	if (current_cmd->type_cmd == 1)
		er_piping_next_cmd(next_cmd, redir);
	if (redir->has_out == -1 && current_cmd->type_cmd == 1)
	{
		dup2(next_cmd->fd[1], STDOUT_FILENO);
		close(next_cmd->fd[1]);
		return (EXIT_SUCCESS);
	}
	else if (redir->has_out != -1 && !redir->filename_failed)
		fd = er_open_file_out(redir);
	if (fd < 0)
	{
		ft_putstr_fd(redir->argv[redir->has_in + 1], 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (EXIT_FAILURE);
	}
	if (redir->has_out != -1 && !redir->filename_failed)
		er_stdout_to_file(fd);
	return (EXIT_SUCCESS);
}

int	er_prepare_stdin(t_cmd *current_cmd, t_redir *redir)
{
	int	fd;

	fd = 0;
	if (current_cmd->fd[0] != 0 && redir->has_in != -1)
		close(current_cmd->fd[0]);
	else if (current_cmd->fd[0] != 0 && redir->has_in == -1)
	{
		dup2(current_cmd->fd[0], STDIN_FILENO);
		close(current_cmd->fd[0]);
	}
	if (redir->has_in != -1 && !redir->filename_failed)
	{
		fd = open(redir->argv[redir->has_in + 1], O_RDONLY, 0644);
		if (fd < 0)
		{
			ft_putstr_fd(redir->argv[redir->has_in + 1], 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (EXIT_FAILURE);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (EXIT_SUCCESS);
}
