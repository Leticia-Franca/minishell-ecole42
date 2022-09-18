/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_final.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 21:19:03 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/27 17:52:16 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ef_free_redir(t_redir *redir)
{
	er_free_matrix(redir->type);
	er_free_matrix(redir->argv);
	free(redir->type);
	free(redir->argv);
	if (redir->commands)
	{
		er_free_matrix(redir->commands);
		free(redir->commands);
	}
}

void	ef_free_redir_envs(t_redir *redir, char **env_matrix)
{
	ef_free_redir(redir);
	er_free_matrix(env_matrix);
	free(env_matrix);
}

void	ef_free_envs(char **env_matrix)
{
	er_free_matrix(env_matrix);
	free(env_matrix);
}

void	ef_treat_exit(t_cmd *cur_cmd, int *saved_std, char **env)
{
	int	len;

	len = ft_strlen(cur_cmd->tokens[0]);
	if (!ft_strncmp(cur_cmd->tokens[0], "exit", len))
	{
		dc_close_stds(saved_std);
		ef_free_envs(env);
	}
}

void	ef_finalize_exec_process(int ret, int *saved_stds)
{
	if (ret == 3)
		ret = 1;
	else if (ret == 132)
		ret = 130;
	g_global.exit_status = ret;
	dc_close_stds(saved_stds);
	fl_free_list();
}
