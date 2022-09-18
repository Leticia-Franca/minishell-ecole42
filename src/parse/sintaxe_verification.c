/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sintaxe_verification.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 12:51:21 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 14:21:33 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	sv_check_sintax_token(char **tokens, int i, int counter_nodes)
{
	if (!tokens[i + 1])
	{
		if ((counter_nodes + 1) < g_global.qty_cmd)
			return (pc_print_error("`|'"));
		else
			return (pc_print_error("`newline'"));
	}
	else if (tokens[i][0] == '<')
	{
		if (!ft_strncmp(tokens[i], "<<", ft_strlen(tokens[i])))
			return (EXIT_SUCCESS);
		else
			return (pc_print_error(tokens[i]));
	}
	else if (tokens[i][0] == '>')
	{
		if (!ft_strncmp(tokens[i], ">>", ft_strlen(tokens[i])))
			return (EXIT_SUCCESS);
		else
			return (pc_print_error(tokens[i]));
	}
	return (EXIT_SUCCESS);
}

int	sv_sintaxe_verification(char **tokens, int counter_nodes)
{
	int	i;

	i = -1;
	while (tokens[++i])
	{
		if (i != 0 && (tokens[i][0] == '<' || tokens[i][0] == '>'))
		{
			if (tokens[i - 1][0] == '<')
				return (pc_print_error(tokens[i]));
			else if (tokens[i - 1][0] == '>')
				return (pc_print_error(tokens[i]));
		}
		if (tokens[i][0] == '<' || tokens[i][0] == '>')
		{
			if (sv_check_sintax_token(tokens, i, counter_nodes) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			else
				continue ;
		}
	}
	return (0);
}
