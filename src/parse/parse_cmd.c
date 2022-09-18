/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/24 22:30:48 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pc_print_error(char *unexpected_token)
{
	g_global.exit_status = 2;
	ft_putstr_fd("bash: sintax error near unexpected token ", 2);
	ft_putendl_fd(unexpected_token, 2);
	return (EXIT_FAILURE);
}

int	pc_count_pipes(char **subcommands, int qty_pipes)
{
	int	counter_subcmd;

	counter_subcmd = 0;
	while (subcommands[counter_subcmd] != NULL)
		counter_subcmd++;
	g_global.qty_cmd = counter_subcmd;
	if (qty_pipes > counter_subcmd || qty_pipes == counter_subcmd)
		return (pc_print_error("'|'"));
	return (0);
}

int	pc_management_parse(char *cmdline)
{
	char	**subcommands;
	int		ret;
	int		qty_pipes;

	ret = 0;
	qty_pipes = 0;
	g_global.qty_cmd = 0;
	g_global.delimiter = NULL;
	subcommands = sm_split_3(cmdline, '|', &qty_pipes);
	ret = pc_count_pipes(subcommands, qty_pipes);
	if (ret == EXIT_FAILURE)
	{
		er_free_matrix(subcommands);
		free(subcommands);
		return (ret);
	}
	ret = pn_create_list(subcommands);
	er_free_matrix(subcommands);
	free(subcommands);
	if (ret != 0)
		return (ret);
	return (0);
}
