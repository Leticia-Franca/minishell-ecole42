/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_minishell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 12:56:27 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	sm_count_words(char *cmdline, char delimiter, int *qty_pipes)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (cmdline[i] != '\0')
	{
		if (cmdline[i] == delimiter)
		{
			if (delimiter == '|')
				(*qty_pipes)++;
			i++;
		}
		else
		{
			count++;
			while (cmdline[i] && cmdline[i] != delimiter)
			{
				if (cmdline[i] == 39 || cmdline[i] == 34)
					su_verify_quote(&i, cmdline, cmdline[i]);
				i++;
			}
		}
	}
	return (count);
}

static char	*sm_make_words(char *word, char *cmdline, int j, int word_ln)
{
	int	i;

	i = 0;
	while (word_ln > 0)
		word[i++] = cmdline[j - word_ln--];
	word[i] = '\0';
	return (word);
}

static char	**sm_split_words(char **res, char *cmdline, char c, int word_qt)
{
	int		i;
	int		j;
	int		word_ln;

	i = 0;
	j = 0;
	while (cmdline[j] && i < word_qt)
	{
		word_ln = 0;
		while (cmdline[j] && cmdline[j] == c)
			j++;
		while (cmdline[j] && cmdline[j] != c)
			su_found_len_word(&word_ln, cmdline, &j);
		res[i] = (char *)malloc(sizeof(char) * (word_ln + 1));
		if (!res[i])
			return (NULL);
		sm_make_words (res[i], cmdline, j, word_ln);
		i++;
	}
	res[i] = 0;
	return (res);
}

char	**sm_split_3(char *cmdline, char delimiter, int *qty_pipes)
{
	char	**fragment;
	int		word_qt;

	if (cmdline == 0)
		return (NULL);
	word_qt = sm_count_words(cmdline, delimiter, qty_pipes);
	fragment = (char **)malloc(sizeof(char *) * (word_qt + 1));
	if (!fragment)
		return (NULL);
	sm_split_words (fragment, cmdline, delimiter, word_qt);
	return (fragment);
}
