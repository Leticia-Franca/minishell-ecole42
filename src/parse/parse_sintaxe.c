/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sintaxe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 14:20:13 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ps_valid_redir(char **subcommand)
{
	int	index;

	index = 0;
	while ((*subcommand)[index])
	{
		if ((*subcommand)[index] == '\'' || (*subcommand)[index] == '\"')
			su_verify_quote(&index, (*subcommand), (*subcommand)[index]);
		else if ((*subcommand)[index] == '<' || (*subcommand)[index] == '>')
		{
			if (index == 0)
			{
				if (((*subcommand)[index + 1] != ' '
					&& (*subcommand)[index + 1] != (*subcommand)[index]))
					pl_insert_spaces(subcommand, &index);
			}
			else if (((*subcommand)[index - 1] != ' '
				&& (*subcommand)[index - 1] != (*subcommand)[index])
				|| ((*subcommand)[index + 1] != ' '
						&& (*subcommand)[index + 1] != (*subcommand)[index]))
				pl_insert_spaces(subcommand, &index);
		}
		index++;
	}
	return (0);
}

int	ps_jump_var_declaration(char **tokens)
{
	int	total_tokens;
	int	index;

	total_tokens = 0;
	index = 0;
	while (tokens[index] && ft_strchr(tokens[index], '='))
		index++;
	while (tokens[index] && tokens[total_tokens])
		total_tokens++;
	return (total_tokens - index);
}

char	**ps_remove_start_var_declares(char **tokens)
{
	int		total_new_tokens;
	char	**new_tokens;
	int		index;
	int		index_new_tokens;

	index = 0;
	index_new_tokens = 0;
	total_new_tokens = ps_jump_var_declaration(tokens);
	new_tokens = (char **)malloc(sizeof(char *) * (total_new_tokens + 1));
	if (ft_strchr(tokens[0], '='))
	{
		while (tokens[index] && ft_strchr(tokens[index], '='))
			index++;
		while (tokens[index] && index_new_tokens < total_new_tokens)
		{
			new_tokens[index_new_tokens] = ft_strdup(tokens[index]);
			index_new_tokens++;
			index++;
		}
		new_tokens[index_new_tokens] = NULL;
	}
	return (new_tokens);
}

static void	ps_check_valid_declarations(t_cmd *temp, int ret)
{
	char	**new_tokens;

	if (ft_strchr(temp->tokens[0], '=') && ret == 0)
	{
		if (!pu_is_var_declare_followed_by_redir(temp->tokens))
		{
			new_tokens = ps_remove_start_var_declares(temp->tokens);
			er_free_matrix(temp->tokens);
			free(temp->tokens);
			temp->tokens = new_tokens;
			new_tokens = NULL;
		}
	}
}

int	ps_validate_sintaxe(t_cmd *temp, int counter_nodes)
{
	char	**new_tokens;
	int		ret;
	int		total_tokens;

	ret = 0;
	total_tokens = 0;
	if (sv_sintaxe_verification(temp->tokens, counter_nodes) != 0)
		return (2);
	ret = pu_are_tokens_declarations(temp->tokens);
	if (ret == 0)
	{
		new_tokens = pr_order_cmd(temp->tokens, &total_tokens);
		free(temp->tokens);
		temp->tokens = new_tokens;
	}
	new_tokens = NULL;
	ps_check_valid_declarations(temp, ret);
	temp->has_redir = pu_has_redir(temp->tokens);
	if ((counter_nodes + 1) < g_global.qty_cmd)
		temp->type_cmd = TYPE_PIPE;
	else
		temp->type_cmd = TYPE_NULL;
	return (0);
}
