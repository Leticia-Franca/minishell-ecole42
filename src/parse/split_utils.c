/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/21 23:11:05 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	su_verify_quote(int *i, char *cmdline, char placeholder_quote)
{
	int	j;

	placeholder_quote = cmdline[*i];
	j = *i + 1;
	while (cmdline[j] && cmdline[j] != placeholder_quote)
		j++;
	if (cmdline[j] == placeholder_quote)
		*i = j;
}

void	su_found_fragment(int *word_ln, char const *cmdline, int *j)
{
	char	size_qt;
	int		aux;
	char	quote;

	if (cmdline[*j] == 39 || cmdline[*j] == 34)
	{
		size_qt = 1;
		quote = cmdline[*j];
		aux = (*j) + 1;
		while (cmdline[aux] && cmdline[aux] != quote)
		{	
			aux++;
			size_qt++;
		}
		if (cmdline[aux] == quote)
		{
			*word_ln += size_qt;
			*j = aux;
		}
	}
}

void	su_found_len_word(int *word_ln, char *cmdline, int *j)
{
	if (cmdline[(*j)] == 39 || cmdline[(*j)] == 34)
		su_found_fragment(word_ln, cmdline, j);
	(*j)++;
	(*word_ln)++;
}
