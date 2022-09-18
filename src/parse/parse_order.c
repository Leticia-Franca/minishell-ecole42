/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_order.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 08:43:37 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/23 21:32:25 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	pr_copy_file(char **tokens, char**new, int ct_tokens, int *i_new)
{
	if ((!pr_is_token_redir(tokens[ct_tokens])
			|| pr_is_token_redir(tokens[ct_tokens])))
	{
		new[(*i_new)] = ft_strdup(tokens[ct_tokens]);
		free(tokens[ct_tokens]);
		tokens[ct_tokens] = NULL;
		(*i_new)++;
	}
}

static int	pr_copy_args(char **tokens, char**new, int *ct_tokens, int *i_new)
{
	if ((((*ct_tokens) != 0 && !pr_is_token_redir(tokens[(*ct_tokens)])
				&& pr_is_token_redir(tokens[(*ct_tokens) - 1]))
			|| pr_is_token_redir(tokens[(*ct_tokens)])))
	{
		(*ct_tokens)++;
		return (0);
	}
	else
	{
		new[(*i_new)] = ft_strdup(tokens[(*ct_tokens)]);
		free(tokens[(*ct_tokens)]);
		tokens[(*ct_tokens)] = NULL;
		(*i_new)++;
		return (1);
	}
}

static void	clear_last_token(char **tokens, int ct_tokens)
{
	free (tokens[ct_tokens]);
	tokens[ct_tokens] = NULL;
}

static char	**init_matrix(char **tokens, int *total_tokens)
{
	char	**new_tokens;

	while (tokens[(*total_tokens)])
		(*total_tokens)++;
	new_tokens = (char **)malloc(sizeof(char *) * ((*total_tokens) + 1));
	new_tokens[(*total_tokens)] = NULL;
	return (new_tokens);
}

char	**pr_order_cmd(char **tokens, int *total_tokens)
{
	char	**newtks;
	int		ct_tokens;
	int		index_new;
	int		two_iter;

	index_new = 0;
	two_iter = -1;
	newtks = init_matrix(tokens, total_tokens);
	while (++two_iter < 2)
	{
		ct_tokens = -1;
		while (++ct_tokens < (*total_tokens))
		{
			if (two_iter < 1)
			{
				if (!pr_copy_args(tokens, newtks, &ct_tokens, &index_new))
					continue ;
			}
			else if (tokens[ct_tokens] && index_new < (*total_tokens))
				pr_copy_file(tokens, newtks, ct_tokens, &index_new);
		}
	}
	clear_last_token(tokens, ct_tokens);
	return (newtks);
}
