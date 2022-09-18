/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_varname.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 14:21:01 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	pv_replace_by_value(char **tokens, int *i, char *var_name)
{
	t_variables	*node_var;

	node_var = el_search_var(g_global.head_env, var_name);
	if (node_var)
		pr_rebuild_string(tokens, node_var->value, i);
	else if (g_global.head_local)
	{
		node_var = el_search_var(g_global.head_local, var_name);
		if (node_var)
			pr_rebuild_string(tokens, node_var->value, i);
		else
			pr_rebuild_string(tokens, " ", i);
	}
	else
		pr_rebuild_string(tokens, " ", i);
}

static	void	pv_replace_varname(char **tokens, int start, int len, int *i)
{
	char	*var_name;
	char	*exit_status;
	int		length;

	(*i) = (*i) - 1;
	exit_status = ft_itoa(g_global.exit_status);
	if ((*tokens)[(*i) + 1] == '?'
		|| ((*tokens)[(*i) + 1] == '{' && (*tokens)[(*i) + 2] == '?'))
	{
		pr_rebuild_string(tokens, exit_status, i);
		free(exit_status);
		return ;
	}
	free(exit_status);
	var_name = ft_substr(*tokens, start, len);
	length = ft_strlen(var_name);
	if (!var_name || !ft_strncmp(var_name, "", length))
	{
		if (var_name && !ft_strncmp(var_name, "", length))
			free(var_name);
		return ;
	}
	pv_replace_by_value(tokens, i, var_name);
	free (var_name);
}

static int	pv_treat_keyed_var(char **tokens, int *i)
{
	int	start;
	int	j;

	if ((*tokens)[(*i)] == '{' && (*tokens)[(*i) + 1] == '}')
	{
		ft_putendl_fd("bash: ${}: incorrect substitution", STDERR_FILENO);
		g_global.exit_status = 1;
		return (-1);
	}
	j = (*i) + 1;
	while ((*tokens)[j] && (*tokens)[j] != '}')
		j++;
	if ((*tokens)[j] == '}')
	{
		start = (*i) + 1;
		pv_replace_varname(tokens, start, j - start, i);
	}
	else
	{
		ft_putendl_fd("ERROR : close key missing", STDERR_FILENO);
		g_global.exit_status = 2;
		return (-1);
	}
	return (0);
}

int	pv_extract_varname(char **tokens, int *i)
{
	int	start;
	int	j;

	if ((*tokens)[(*i)] != '{')
	{
		j = (*i);
		while ((*tokens)[j] && (ft_isalnum((*tokens)[j])
				|| (*tokens)[j] == '_'))
			j++;
		start = (*i);
		pv_replace_varname(tokens, start, j - start, i);
	}
	else if ((*tokens)[(*i)] == '{')
	{
		if (pv_treat_keyed_var(tokens, i) == -1)
			return (-1);
	}
	return (0);
}
