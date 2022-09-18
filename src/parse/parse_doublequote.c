/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_doublequote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:20:11 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pd_is_token_redir(char *token)
{
	if (!token)
		return (0);
	if (!ft_strncmp(token, "<", ft_strlen(token))
		|| !ft_strncmp(token, "<<", ft_strlen(token))
		|| !ft_strncmp(token, ">", ft_strlen(token))
		|| !ft_strncmp(token, ">>", ft_strlen(token)))
		return (1);
	return (0);
}

int	pd_found_closed_quote(char **tokens, int i)
{
	int	j;

	j = i + 1;
	while ((*tokens)[j] && (*tokens)[j] != 34)
		j++;
	if ((*tokens)[j] == 34)
		return (1);
	else
		return (0);
}

int	pd_is_between_double(char **tokens, int i)
{
	int	count_dq;
	int	index;
	int	ret;

	count_dq = 0;
	index = 0;
	while ((*tokens)[index] && index < i)
	{
		if ((*tokens)[index] == 34)
			count_dq++;
		index++;
	}
	if (count_dq % 2 == 0)
		return (0);
	else
	{
		ret = pd_found_closed_quote(tokens, i);
		return (ret);
	}
	return (0);
}
