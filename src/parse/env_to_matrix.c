/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 08:47:44 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/27 16:44:18 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_is_strcmp_true(char *name_list, char *name_input)
{
	if (ft_strncmp(name_list, name_input, ft_strlen(name_list)) == 0
		&& ft_strncmp(name_list, name_input, ft_strlen(name_input)) == 0)
		return (1);
	return (0);
}

int	el_counter_envs(void)
{
	int			count;
	t_variables	*tmp;

	tmp = g_global.head_env;
	count = 0;
	while (tmp->next != NULL)
	{
		count++;
		tmp = tmp->next;
	}
	count++;
	return (count);
}

char	**el_env_to_matrix(void)
{
	t_variables	*tmp;
	char		**envs;
	char		*env_name;
	int			total_envs;
	int			index;

	index = 0;
	tmp = g_global.head_env;
	total_envs = el_counter_envs();
	envs = (char **)malloc(sizeof(char *) * (total_envs + 1));
	while (index < total_envs)
	{
		env_name = ft_strjoin(tmp->name, "=");
		envs[index] = ft_strjoin(env_name, tmp->value);
		free(env_name);
		tmp = tmp->next;
		index++;
	}
	envs[index] = NULL;
	return (envs);
}
