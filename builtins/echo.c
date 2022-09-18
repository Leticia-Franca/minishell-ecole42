/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 19:33:58 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/25 16:21:45 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	echo_print_tokens(char **tokens, int i)
{
	while (tokens[i])
	{
		ft_putstr_fd(tokens[i], 1);
		if (tokens[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
}

void	ft_echo(char **tokens)
{
	int	i;
	int	has_newline;

	has_newline = 1;
	i = 1;
	if (tokens[0] && tokens[1] == NULL)
	{
		ft_putchar_fd('\n', 1);
		g_global.exit_status = 0;
		return ;
	}
	while (tokens[i] && ft_strncmp(tokens[i], "-n", 2) == 0)
	{
		has_newline = 0;
		i++;
	}
	if (!tokens[i])
	{
		g_global.exit_status = 0;
		return ;
	}
	echo_print_tokens(tokens, i);
	if (has_newline == 1)
		ft_putchar_fd('\n', 1);
	g_global.exit_status = 0;
}
