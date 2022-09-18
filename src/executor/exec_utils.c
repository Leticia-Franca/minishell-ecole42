/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 21:19:12 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/27 16:45:15 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	eu_open_pipe(t_cmd *current_cmd, t_cmd *next_cmd)
{
	if (current_cmd->type_cmd == 1)
	{
		next_cmd->pre_flag = 1;
		pipe(next_cmd->fd);
	}
}

static int	eu_path_cmd(struct stat *s, char *pathcmd, char **paths)
{
	if (stat(pathcmd, s) == 0)
	{
		er_free_matrix(paths);
		free(paths);
		return (1);
	}
	return (0);
}

char	*eu_find_cmdpath(char *line_paths, char *cmd)
{
	struct stat	s;
	char		**paths;
	char		*dir;
	char		*pathcmd;
	int			index;

	index = -1;
	if (stat(cmd, &s) == 0)
		return (ft_strdup(cmd));
	paths = ft_split(line_paths, ':');
	while (paths[++index])
	{
		dir = ft_strjoin(paths[index], "/");
		pathcmd = ft_strjoin(dir, cmd);
		free(dir);
		if (eu_path_cmd(&s, pathcmd, paths) == 1)
			return (pathcmd);
		free (pathcmd);
	}
	er_free_matrix(paths);
	free(paths);
	return (NULL);
}

int	eu_is_here_interrupted(int ret, t_redir *redir, char **env_matrix)
{
	if (ret == 130 || ret == 2)
	{
		ef_free_redir_envs(redir, env_matrix);
		return (1);
	}
	return (0);
}

int	eu_is_cmd_piped(t_cmd *current_cmd)
{
	if ((current_cmd->type_cmd == 0
			&& current_cmd->pre_flag == 0))
		return (0);
	return (1);
}
