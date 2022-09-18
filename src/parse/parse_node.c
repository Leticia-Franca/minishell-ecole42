/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/23 21:25:46 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_cmd	*pn_create_node(void)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	node->next = NULL;
	node->tokens = NULL;
	node->pre_flag = 0;
	node->has_redir = -1;
	node->fd[0] = 0;
	node->fd[1] = 0;
	return (node);
}

static t_cmd	*create_current_cmd_node(void)
{
	t_cmd	*temp;

	if (g_global.head == NULL)
	{
		g_global.head = pn_create_node();
		temp = g_global.head;
		temp->pre_flag = 0;
	}
	else
	{
		temp = g_global.head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = pn_create_node();
		temp = temp->next;
		temp->pre_flag = 1;
	}
	return (temp);
}

static void	pn_remove_quotes(t_cmd *temp)
{
	int	count_token;

	count_token = -1;
	while (temp->tokens[++count_token])
	{
		rq_remove_quotes(&temp->tokens[count_token]);
	}
}

static int	pn_tokenize_expand_vars(t_cmd *temp, char *line_tokens)
{
	int	ret;
	int	count_token;
	int	qty_void;

	count_token = 0;
	qty_void = 0;
	temp->tokens = sm_split_3(line_tokens, ' ', &qty_void);
	while (temp->tokens[count_token])
	{
		ret = pl_verify_expand(&temp->tokens[count_token]);
		if (ret != 0)
			return (ret);
		count_token++;
	}
	return (0);
}

int	pn_create_list(char **subcommands)
{
	t_cmd	*temp;
	int		idx_subcmd;
	int		ret;
	int		counter_nodes;

	idx_subcmd = -1;
	ret = 0;
	counter_nodes = 0;
	while (subcommands[++idx_subcmd])
	{
		temp = create_current_cmd_node();
		ps_valid_redir(&subcommands[idx_subcmd]);
		ret = pn_tokenize_expand_vars(temp, subcommands[idx_subcmd]);
		if (ret != 0)
			return (ret);
		ret = ps_validate_sintaxe(temp, counter_nodes);
		if (ret != 0)
		{
			g_global.exit_status = ret;
			return (ret);
		}
		pn_remove_quotes(temp);
		counter_nodes++;
	}
	return (0);
}
