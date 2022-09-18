/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 20:14:51 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 14:20:37 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pu_has_redir(char **tokens)
{
	int	index;

	index = 0;
	while (tokens[index])
	{
		if (tokens[index][0] == '<' || tokens[index][0] == '>')
			return (1);
		index++;
	}
	return (-1);
}

int	pu_are_tokens_declarations(char **tokens)
{
	int	counter_tokens;

	counter_tokens = 0;
	while (tokens[counter_tokens])
	{
		if (!ft_strchr(tokens[counter_tokens], '='))
			return (0);
		counter_tokens++;
	}
	return (1);
}

int	pu_is_var_declare_followed_by_redir(char **tokens)
{
	int	index;

	index = 0;
	while (tokens[index] && ft_strchr(tokens[index], '='))
		index++;
	if (tokens[index])
	{
		if (!pr_is_token_redir(tokens[index]))
			return (0);
	}
	return (1);
}
