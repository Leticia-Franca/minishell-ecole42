/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:24:10 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pl_verify_expand(char **tokens)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while ((*tokens)[i])
	{
		if ((*tokens)[i] == 39)
		{
			if (i == 0 || !pd_is_between_double(tokens, i))
				su_verify_quote(&i, *tokens, (*tokens)[i]);
		}
		else if ((*tokens)[i] == '$')
		{
			i++;
			ret = pv_extract_varname(tokens, &i);
			if (ret != 0)
				return (ret);
		}
		if ((*tokens)[i])
			i++;
	}
	return (0);
}

char	*pl_rebuild_spaced(int index, char *subcommand, int start)
{
	char	*substring;
	char	*str_addedspace;
	char	*spaced_string;

	substring = ft_substr(subcommand, start, index - start);
	str_addedspace = ft_strjoin(substring, " ");
	free(substring);
	substring = ft_substr(subcommand, index, ft_strlen(subcommand) - index);
	spaced_string = ft_strjoin(str_addedspace, substring);
	free(substring);
	free(str_addedspace);
	return (spaced_string);
}

static void	pl_insert_before_after(char **subcommand, int *index)
{
	char	*rebuilt_once;
	char	*rebuilt_twice;

	rebuilt_once = pl_rebuild_spaced(*index, *subcommand, 0);
	rebuilt_twice = pl_rebuild_spaced((*index) + 2, rebuilt_once, 0);
	free(rebuilt_once);
	free(*subcommand);
	(*subcommand) = rebuilt_twice;
	(*index)++;
}

void	pl_insert_spaces(char **subcommand, int *index)
{
	char	*old_subcommand;
	char	before;
	char	after;

	before = (*subcommand)[(*index) - 1];
	after = (*subcommand)[(*index) + 1];
	if ((before != ' ' && after != ' ') && (before != '<'
			&& after != '<') && before != '>' && after != '>')
		pl_insert_before_after(subcommand, index);
	else if (*index != 0 && (before != ' ' && before != '<' && (before != '>')))
	{
		old_subcommand = (*subcommand);
		(*subcommand) = pl_rebuild_spaced(*index, *subcommand, 0);
		free(old_subcommand);
		(*index)++;
	}
	else if (*index != 0 && (after != ' ' && after != '<') && after != '>')
	{
		old_subcommand = (*subcommand);
		(*subcommand) = pl_rebuild_spaced((*index) + 1, (*subcommand), 0);
		free(old_subcommand);
	}
}
