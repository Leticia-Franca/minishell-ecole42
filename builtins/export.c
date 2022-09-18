/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/27 09:16:44 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	export_creat_var(char **tokens, int i, t_variables *tmp)
{
	t_variables	*node;
	char		**var_content;

	if (!g_global.head_local)
		return ;
	node = el_search_var(g_global.head_local, tokens[i]);
	if (!node)
		return ;
	var_content = malloc(sizeof(char *) * 3);
	var_content[0] = ft_strdup(node->name);
	var_content[1] = ft_strdup(node->value);
	var_content[2] = NULL;
	tmp = g_global.head_env;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = el_var_node(var_content);
	if (ft_strncmp(tokens[i], var_content[0], ft_strlen(var_content[0])) == 0
		&& ft_strncmp(tokens[i], var_content[0], ft_strlen(tokens[i])) == 0)
		unset_delete_var(&g_global.head_local, var_content[0]);
	er_free_matrix(var_content);
	free(var_content);
}

static void	export_local_to_env(t_variables *tmp, char **new_var)
{
	t_variables	*tmp_env;

	tmp_env = g_global.head_env;
	while (tmp_env->next != NULL)
		tmp_env = tmp_env->next;
	tmp_env->next = el_var_node(new_var);
	free(tmp->value);
	tmp->value = ft_strdup(new_var[1]);
	unset_delete_var(&g_global.head_local, new_var[0]);
}

static void	export_to_env(char **new_var)
{
	t_variables	*tmp;

	tmp = NULL;
	if (g_global.head_local)
		tmp = el_search_var(g_global.head_local, new_var[0]);
	if (!tmp)
	{
		tmp = g_global.head_env;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = el_var_node(new_var);
	}
	else
		export_local_to_env(tmp, new_var);
}

void	ft_export(char **tokens)
{
	t_variables	*tmp;
	char		**new_var;
	int			i;

	i = 0;
	while (tokens[++i])
	{
		if (ft_strchr(tokens[i], '='))
		{
			new_var = ft_split(tokens[i], '=');
			tmp = el_search_var(g_global.head_env, new_var[0]);
			if (tmp)
				ev_change_value(tmp, new_var);
			else
			{
				export_to_env(new_var);
				er_free_matrix(new_var);
				free(new_var);
			}
		}
		else
			export_creat_var(tokens, i, tmp);
	}
	g_global.exit_status = 0;
}
