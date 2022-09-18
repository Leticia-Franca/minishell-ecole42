/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_parse_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 09:02:44 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 20:52:55 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*sr_set_type_redir(char *cmdline, int i)
{
	char	*redir_type;

	if (cmdline[i] == '<' && cmdline[i + 1] == '<')
		redir_type = ft_strdup("HEREDOC");
	else if (cmdline[i] == '<' && cmdline[i + 1] != '<')
		redir_type = ft_strdup("REDIR_IN");
	else if (cmdline[i] == '>' && cmdline[i + 1] != '>')
		redir_type = ft_strdup("REDIR_OUT");
	else if (cmdline[i] == '>' && cmdline[i + 1] == '>')
		redir_type = ft_strdup("DB_REDIR");
	return (redir_type);
}

void	sr_set_index_valid_file(t_redir *redir)
{
	int	index;
	int	len;

	index = 0;
	while (redir->type[index])
	{
		len = ft_strlen(redir->type[index]);
		if (!ft_strncmp(redir->type[index], "HEREDOC", len))
			redir->has_in = index;
		else if (!ft_strncmp(redir->type[index], "REDIR_IN", len))
			redir->has_in = index;
		else if (!ft_strncmp(redir->type[index], "REDIR_OUT", len))
			redir->has_out = index;
		else if (!ft_strncmp(redir->type[index], "DB_REDIR", len))
			redir->has_out = index;
		index++;
	}
}

static void	sr_compare_type_redir(t_redir *redir, char *cmdline, int *j, int *i)
{
	int	len;

	len = ft_strlen(redir->argv[(*j)]);
	if ((*j) == 0 && ft_strncmp(redir->argv[(*j)], "no_command", len))
		redir->commands = ft_split(redir->argv[(*j)], ' ');
	redir->type[(*j)] = sr_set_type_redir(cmdline, (*i));
	if (!ft_strncmp(redir->type[(*j)], "DB_REDIR", len)
		|| !ft_strncmp(redir->type[(*j)], "HEREDOC", len))
	{
		if (!ft_strncmp(redir->type[(*j)], "HEREDOC", len))
			redir->has_heredoc = (*j);
		(*i)++;
	}
	(*j)++;
}

static void	sr_treat_last_file(t_redir *redir, char *tmp_argv, int j)
{
	redir->argv[j] = ft_strtrim(tmp_argv, " ");
	if (tmp_argv)
		free(tmp_argv);
	redir->type[j] = 0;
	redir->argv[j + 1] = NULL;
	redir->argc = j + 1;
	sr_set_index_valid_file(redir);
}

int	sr_parse_redir(t_redir *redir, char *cmdline)
{
	int		start;
	int		i;
	int		j;
	char	*tmp_argv;

	i = -1;
	j = 0;
	start = 0;
	while (cmdline[++i])
	{
		if (ft_strchr("><", cmdline[i]))
		{
			tmp_argv = ft_substr(cmdline, start, i - start);
			if (ft_strlen(tmp_argv))
				redir->argv[j] = ft_strtrim(tmp_argv, " ");
			else
				redir->argv[j] = ft_strdup("no_command");
			free(tmp_argv);
			sr_compare_type_redir(redir, cmdline, &j, &i);
			start = i + 1;
		}
	}
	tmp_argv = ft_substr(cmdline, start, i - start);
	sr_treat_last_file(redir, tmp_argv, j);
	return (0);
}
