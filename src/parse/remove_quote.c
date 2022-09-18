/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 19:19:39 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/25 16:33:17 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	rq_count_quotes(char **token, char *quote_to_remove)
{
	int	index;
	int	total_quotes;

	index = 0;
	total_quotes = 0;
	while ((*token)[index])
	{
		if ((*token)[index] == 39 || (*token)[index] == 34)
		{
			if ((*quote_to_remove) == 0)
			{
				(*quote_to_remove) = (*token)[index];
				total_quotes++;
			}
			else if ((*token)[index] == (*quote_to_remove))
				total_quotes++;
		}
		index++;
	}
	return (total_quotes);
}

static void	rq_copy_odd_quote(char *n_token, char **token, int i, int *n_i)
{
	n_token[(*n_i)] = (*token)[i];
	(*n_i)++;
}

static char	*rq_cut_quotes(char **token, int len_new, char quote, int total_q)
{
	char	*new_token;
	int		index;
	int		index_new_token;
	int		nb_q;

	index = -1;
	index_new_token = 0;
	nb_q = total_q;
	new_token = (char *)malloc(sizeof(char) * (len_new + 1));
	new_token[len_new] = '\0';
	while ((*token)[++index])
	{
		if ((*token)[index] != quote)
		{
			new_token[index_new_token] = (*token)[index];
			index_new_token++;
		}
		else
		{
			if (nb_q % 2 != 0 && total_q == 1)
				rq_copy_odd_quote(new_token, token, index, &index_new_token);
			total_q--;
		}
	}
	return (new_token);
}

void	rq_remove_quotes(char **token)
{
	char	*new_token;
	int		len_new;
	char	quote;
	int		total_quotes;

	quote = 0;
	total_quotes = 0;
	total_quotes = rq_count_quotes(token, &quote);
	len_new = ft_strlen(*token) - total_quotes;
	if (total_quotes % 2 != 0)
		len_new++;
	new_token = rq_cut_quotes(token, len_new, quote, total_quotes);
	free((*token));
	(*token) = new_token;
}
