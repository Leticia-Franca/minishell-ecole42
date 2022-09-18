/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:10:25 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	fl_free_list(void)
{
	t_cmd	*temp;
	int		count;
	int		i;

	count = 0;
	while (count < g_global.qty_cmd)
	{
		i = 0;
		temp = g_global.head->next;
		g_global.head->next = NULL;
		while (g_global.head->tokens[i])
		{
			free(g_global.head->tokens[i]);
			g_global.head->tokens[i] = NULL;
			i++;
		}
		free(g_global.head->tokens[i]);
		free(g_global.head->tokens);
		free(g_global.head);
		g_global.head = temp;
		count++;
	}
}

void	fl_free_var_list(t_variables **head)
{
	t_variables	*current;

	current = (*head);
	while ((*head)->next != NULL)
	{
		(*head) = (*head)->next;
		free(current->name);
		free(current->value);
		current->next = NULL;
		free(current);
		current = (*head);
	}
	if ((*head)->next == NULL)
	{
		free(current->name);
		free(current->value);
		free(current->next);
	}
	free(*head);
	(*head) = NULL;
}

void	er_free_matrix(char **matrix)
{
	int	counter;

	counter = 0;
	while (matrix[counter])
	{
		free(matrix[counter]);
		counter++;
	}
	free(matrix[counter]);
}
