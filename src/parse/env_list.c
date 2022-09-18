/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/27 16:45:30 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_variables	*el_var_node(char **content_var)
{
	t_variables	*node;

	node = (t_variables *)malloc(sizeof(t_variables));
	node->name = ft_strdup(content_var[0]);
	node->value = ft_strdup(content_var[1]);
	node->next = NULL;
	return (node);
}

static int	el_del_last_node(t_variables **tmp, t_variables **pn, char *name)
{
	if ((*pn)->next == NULL)
	{
		if (ft_strncmp((*pn)->name, name, ft_strlen((*pn)->name)) == 0
			&& ft_strncmp((*pn)->name, name, ft_strlen(name)) == 0)
		{
			free((*pn)->name);
			free((*pn)->value);
			(*tmp)->next = NULL;
			free((*pn));
			return (1);
		}
	}
	return (0);
}

static int	el_src_var_del(t_variables **tmp, t_variables **pn, char *name)
{
	while ((*pn)->next != NULL)
	{
		if (ft_strncmp((*pn)->name, name, ft_strlen((*pn)->name)) == 0
			&& ft_strncmp((*pn)->name, name, ft_strlen(name)) == 0)
		{
			free((*pn)->name);
			free((*pn)->value);
			(*tmp)->next = (*pn)->next;
			(*pn)->next = NULL;
			free((*pn));
			return (1);
		}
		(*tmp) = (*pn);
		(*pn) = (*pn)->next;
	}
	return (0);
}

int	unset_delete_var(t_variables **head, char *name)
{
	t_variables	*tmp;
	t_variables	*ptr_next;

	tmp = (*head);
	ptr_next = NULL;
	if (ft_is_strcmp_true(tmp->name, name))
	{
		(*head) = (*head)->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
		return (1);
	}
	else
	{
		ptr_next = tmp->next;
		if (el_src_var_del(&tmp, &ptr_next, name) == 1)
			return (1);
		else
		{
			if (el_del_last_node(&tmp, &ptr_next, name) == 1)
				return (1);
		}
	}
	return (0);
}

t_variables	*el_search_var(t_variables *head, char *name)
{
	t_variables	*tmp;

	tmp = head;
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(tmp->name, name, ft_strlen(name))
			&& !ft_strncmp(tmp->name, name, ft_strlen(tmp->name)))
			return (tmp);
		tmp = tmp->next;
	}
	if (tmp->next == NULL && (!ft_strncmp(tmp->name, name, ft_strlen(name)))
		&& !ft_strncmp(tmp->name, name, ft_strlen(tmp->name)))
		return (tmp);
	else
		return (NULL);
}
