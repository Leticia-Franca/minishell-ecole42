/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 22:54:41 by cleticia          #+#    #+#             */
/*   Updated: 2022/05/24 02:13:09 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	dc_close_stds(int *saved_stds)
{
	close(saved_stds[0]);
	close(saved_stds[1]);
}

void	dc_dup_stds(int *saved_stds)
{
	dup2(saved_stds[0], STDIN_FILENO);
	dup2(saved_stds[1], STDOUT_FILENO);
}
