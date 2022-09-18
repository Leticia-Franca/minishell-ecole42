/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_read_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 12:27:42 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/27 17:04:10 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	eh_here_loop(int tmp_fd, char *delimiter)
{
	char	*tmp;

	while (1)
	{
		tmp = readline("> ");
		if (!tmp)
		{
			ft_putendl_fd("Temporary file error.", STDERR_FILENO);
			close(tmp_fd);
			return (3);
		}
		if (tmp)
		{
			if (ft_strncmp(tmp, delimiter, ft_strlen(tmp)))
				ft_putendl_fd(tmp, tmp_fd);
			if (!ft_strncmp(tmp, delimiter, ft_strlen(tmp)))
			{
				free(tmp);
				close(tmp_fd);
				break ;
			}
		}
		free(tmp);
	}
	return (0);
}

void	eh_free_child_heredoc(t_redir *redir, char **env_matrix)
{
	er_free_matrix(env_matrix);
	free(env_matrix);
	fl_free_list();
	fl_free_var_list(&g_global.head_env);
	if (g_global.head_local)
		fl_free_var_list(&g_global.head_local);
	er_free_matrix(redir->type);
	er_free_matrix(redir->argv);
	er_free_matrix(redir->commands);
	if (redir->filename_failed)
		free(redir->filename_failed);
	free(redir->type);
	free(redir->argv);
	free(redir->commands);
	free(redir);
}

static int	eh_fork_heredoc(t_redir *redir, char **envs, int *tfd)
{
	int		pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == 0)
	{
		eh_free_child_heredoc(redir, envs);
		signal(SIGINT, eh_interrupt_heredoc);
		ret = eh_here_loop((*tfd), g_global.delimiter);
		rl_clear_history();
		free(g_global.delimiter);
		exit(ret);
	}
	else
		waitpid(pid, &status, 0);
	if (g_global.delimiter)
	{
		free(g_global.delimiter);
		g_global.delimiter = NULL;
	}
	if (WIFEXITED(status) && (WEXITSTATUS(status) == 130
			|| WEXITSTATUS(status) == 3))
		return (WEXITSTATUS(status));
	return (EXIT_SUCCESS);
}

int	eh_create_tmp_file(void)
{
	int	tmp_fd;

	tmp_fd = open("./tmp_heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (tmp_fd == -1)
		ft_putendl_fd("Error: Redirect", 2);
	return (tmp_fd);
}

int	eh_read_heredoc(t_redir *redir, char **env_matrix, int *tmp_fd)
{
	int		index;
	int		ret;
	int		len;

	index = -1;
	ret = 0;
	while (redir->type[++index])
	{
		len = ft_strlen(redir->type[index]);
		if ((!ft_strncmp(redir->type[index], "HEREDOC", len)))
		{
			g_global.delimiter = ft_strdup(redir->argv[index + 1]);
			if ((*tmp_fd) != 0)
				close((*tmp_fd));
			(*tmp_fd) = eh_create_tmp_file();
			ret = eh_fork_heredoc(redir, env_matrix, tmp_fd);
			if (ret != EXIT_SUCCESS)
				return (ret);
			(*tmp_fd) = open("./tmp_heredoc.txt", O_RDONLY);
		}
	}
	return (EXIT_SUCCESS);
}
