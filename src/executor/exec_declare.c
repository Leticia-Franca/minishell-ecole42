/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_declare.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 21:33:39 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 20:01:47 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ev_is_valid_declare(t_cmd *current_cmd)
{
	int	ret;

	ret = 0;
	if (ft_strchr(current_cmd->tokens[0], '=')
		&& (current_cmd->type_cmd == TYPE_NULL
			&& current_cmd->pre_flag == 0))
			ret = 1;
	return (ret);
}

void	ev_change_value(t_variables *tmp, char **content_var)
{
	free(tmp->value);
	tmp->value = ft_strdup(content_var[1]);
	er_free_matrix(content_var);
	free(content_var);
}

static void	ev_create_local_var(char **content_var)
{
	t_variables	*tmp;

	tmp = NULL;
	if (g_global.head_local)
	{
		tmp = g_global.head_local;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = el_var_node(content_var);
	}
	else
		g_global.head_local = el_var_node(content_var);
	er_free_matrix(content_var);
	free(content_var);
}

int	ev_declare_var(char **tokens)
{
	int			i;
	t_variables	*tmp;
	char		**content_var;

	i = 0;
	tmp = NULL;
	while (tokens[i] && ft_strchr(tokens[i], '='))
	{
		content_var = ft_split(tokens[i], '=');
		tmp = el_search_var(g_global.head_env, content_var[0]);
		if (!tmp && (g_global.head_local != NULL))
			tmp = el_search_var(g_global.head_local, content_var[0]);
		if (tmp)
			ev_change_value(tmp, content_var);
		else
			ev_create_local_var(content_var);
		i++;
	}
	return (EXIT_SUCCESS);
}
