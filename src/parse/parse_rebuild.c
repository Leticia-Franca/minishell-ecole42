/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rebuild.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:38:25 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pr_is_token_redir(char *token)
{
	int	len;

	if (!token)
		return (0);
	len = ft_strlen(token);
	if (!ft_strncmp(token, "<", len) || !ft_strncmp(token, "<<", len)
		|| !ft_strncmp(token, ">", len) || !ft_strncmp(token, ">>", len))
		return (1);
	return (0);
}

char	*pr_join_rest(char **tokens, int *index, char *join, int question)
{
	char	*new;
	int		start;
	size_t	len;
	char	*rebuild;

	start = 0;
	len = 0;
	if ((*tokens)[(*index)] == '}' || !ft_isalnum((*tokens)[(*index)]
			|| ((*tokens)[(*index)] == '?' && question == 1)))
	{
		if ((*tokens)[(*index)] == 34 || (*tokens)[(*index)] == 39
			|| (*tokens)[(*index)] == '$')
			start = (*index);
		else
			start = (*index) + 1;
	}
	else
		start = (*index);
	while ((*tokens)[(*index)])
		(*index)++;
	len = (*index) - start;
	new = ft_substr(*tokens, start, len);
	rebuild = ft_strjoin(join, new);
	free(new);
	return (rebuild);
}

static void	pr_treat_key(char **tokens, int *index_rest)
{
	int	has_key;

	has_key = 0;
	if ((*tokens)[(*index_rest) + 1] == '{')
		has_key = 1;
	if (has_key == 1)
	{
		while ((*tokens)[(*index_rest)] && (*tokens)[(*index_rest)] != '}')
			(*index_rest)++;
	}
	else if (has_key == 0)
	{
		(*index_rest)++;
		while ((*tokens)[(*index_rest)]
				&& (ft_isalnum((*tokens)[(*index_rest)])
					|| (*tokens)[(*index_rest)] == '_'))
			(*index_rest)++;
	}
}

static char	*pr_join_before_var(char **tokens, int index_dolar, char *value)
{
	char	*new;
	char	*join;

	new = ft_substr(*tokens, 0, index_dolar);
	join = ft_strjoin(new, value);
	free(new);
	new = join;
	join = ft_strtrim(new, " ");
	free(new);
	return (join);
}

void	pr_rebuild_string(char **tokens, char *value, int *index_dolar)
{
	char	*join;
	char	*rebuild;
	int		is_question_mark;
	int		index_rest;
	int		new_index_expanded;

	new_index_expanded = 0;
	index_rest = (*index_dolar);
	is_question_mark = 0;
	if ((*tokens)[index_rest + 1] == '?')
		is_question_mark = 1;
	if (index_rest != 0)
		join = pr_join_before_var(tokens, (*index_dolar), value);
	else
		join = ft_strdup(value);
	new_index_expanded = (ft_strlen(join) - 1);
	pr_treat_key(tokens, &index_rest);
	if ((*tokens)[index_rest] != '\0')
		rebuild = pr_join_rest(tokens, &index_rest, join, is_question_mark);
	else
		rebuild = ft_strdup(join);
	(*index_dolar) = new_index_expanded;
	free(*tokens);
	free(join);
	(*tokens) = rebuild;
}
