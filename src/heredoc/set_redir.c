/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 20:58:51 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	sr_open_fd(t_redir *redir, int i, int len)
{
	int	fd;

	fd = 0;
	if (!ft_strncmp(redir->type[i - 1], "REDIR_OUT", len))
		fd = open(redir->argv[i], O_WRONLY | O_CREAT | O_TRUNC, 0744);
	else if (!ft_strncmp(redir->type[i - 1], "DB_REDIR", len))
		fd = open(redir->argv[i], O_WRONLY | O_CREAT | O_APPEND, 0744);
	else if (!ft_strncmp(redir->type[i - 1], "REDIR_IN", len))
	{
		fd = open(redir->argv[i], O_RDONLY, 0644);
		if (fd == -1)
		{
			if (!redir->filename_failed)
				redir->filename_failed = ft_strdup(redir->argv[i]);
		}
	}
	return (fd);
}

int	sr_open_unnecessary_file(t_redir *redir)
{
	int	i;
	int	fd;
	int	len;

	i = 1;
	fd = 0;
	while (i <= redir->argc - 1)
	{
		len = ft_strlen(redir->type[i - 1]);
		if (i > 1 && fd > 0)
			close(fd);
		fd = sr_open_fd(redir, i, len);
		i++;
	}
	if (fd > 0)
		close (fd);
	return (0);
}

void	sr_init_redir(t_redir *redir, char *cmdline)
{
	int	cnt_redir;
	int	index;

	cnt_redir = 0;
	index = 0;
	while (cmdline[index])
	{
		if (ft_strchr("><", cmdline[index])
			&& !ft_strchr("><", cmdline[index + 1]))
			cnt_redir++;
		index++;
	}
	redir->has_in = -1;
	redir->has_out = -1;
	redir->has_heredoc = -1;
	redir->argc = cnt_redir + 1;
	redir->argv = (char **)malloc(sizeof(char *) * (redir->argc + 1));
	redir->type = (char **)malloc(sizeof(char *) * (redir->argc + 1));
	redir->filename_failed = NULL;
	redir->commands = NULL;
}

char	*sr_one_line(char **tokens)
{
	char	*cmdline;
	char	*joined;
	char	*tmp;
	int		count_tokens;

	count_tokens = 0;
	while (tokens[count_tokens])
	{
		if (count_tokens == 0)
			joined = ft_strdup(tokens[count_tokens]);
		else
		{
			tmp = ft_strjoin(joined, " ");
			free(joined);
			joined = ft_strjoin(tmp, tokens[count_tokens]);
			free(tmp);
		}
		count_tokens++;
	}
	cmdline = ft_strdup(joined);
	free(joined);
	return (cmdline);
}

t_redir	*sr_treat_redir(t_cmd *current_cmd, int *ret)
{
	t_redir	*redir;
	char	*line;

	redir = (t_redir *)malloc(sizeof(t_redir));
	line = sr_one_line(current_cmd->tokens);
	sr_init_redir(redir, line);
	sr_parse_redir(redir, line);
	(*ret) = sr_open_unnecessary_file(redir);
	free (line);
	return (redir);
}
