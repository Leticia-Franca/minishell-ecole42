/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/27 16:44:52 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	unset_nonexistent_var(char **tokens, int index)
{
	if (ft_strncmp(tokens[index], " ", ft_strlen(tokens[index])))
	{
		ft_putstr_fd("bash: unset: ", STDERR_FILENO);
		ft_putstr_fd(tokens[index], STDERR_FILENO);
		ft_putendl_fd(": isn't a valid identifier", STDERR_FILENO);
		g_global.exit_status = 1;
		return (1);
	}
	return (0);
}

int	unset_remove_var(char **tokens, int index)
{
	t_variables	*node;

	node = NULL;
	node = el_search_var(g_global.head_env, tokens[index]);
	if (node)
	{
		unset_delete_var(&g_global.head_env, node->name);
		if (g_global.head_local)
		{
			node = el_search_var(g_global.head_local, tokens[index]);
			if (node)
				unset_delete_var(&g_global.head_local, node->name);
		}
		return (0);
	}
	if (!node && g_global.head_local)
		node = el_search_var(g_global.head_local, tokens[index]);
	if (!node && unset_nonexistent_var(tokens, index))
		return (1);
	if (!node)
		return (0);
	unset_delete_var(&g_global.head_local, node->name);
	return (0);
}

static int	ft_unset_error(char **tokens, int index)
{
	if (!ft_strncmp(tokens[index], "<", strlen(tokens[index]))
		|| (!ft_strncmp(tokens[index], ">", strlen(tokens[index]))
			|| (!ft_strncmp(tokens[index], "<<", strlen(tokens[index]))
				|| (!ft_strncmp(tokens[index], ">>", strlen(tokens[index]))))))
	{
		if (tokens[index + 1] != NULL)
		{
			ft_putstr_fd("bash: ", STDERR_FILENO);
			ft_putstr_fd(tokens[index + 1], STDERR_FILENO);
			ft_putendl_fd(": Inexistent file or directory", STDERR_FILENO);
			g_global.exit_status = 1;
			return (1);
		}
	}
	return (0);
}

void	ft_unset(char **tokens)
{
	int	index;
	int	ret;

	ret = 0;
	index = 1;
	if (tokens[index] == NULL)
	{
		g_global.exit_status = 0;
		return ;
	}
	while (tokens[index] != NULL)
	{
		if (ft_unset_error(tokens, index) == 1)
			return ;
		index++;
	}
	index = 1;
	while (tokens[index] != NULL)
	{
		ret = unset_remove_var(tokens, index);
		if (ret == 1)
			return ;
		index++;
	}
	g_global.exit_status = 0;
}
