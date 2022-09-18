/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 21:41:28 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/21 22:24:07 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	cd_error_excess_args(char **tokens)
{
	int			n_argc;

	n_argc = 0;
	while (tokens[n_argc])
		n_argc++;
	if (n_argc > 2)
	{
		ft_putendl_fd("bash: cd: excessive arguments number", STDERR_FILENO);
		g_global.exit_status = 1;
		return (1);
	}
	return (0);
}

void	cd_old_pwd(void)
{
	t_variables	*old;
	t_variables	*current;

	old = el_search_var(g_global.head_env, "OLDPWD");
	current = el_search_var(g_global.head_env, "PWD");
	free(old->value);
	old->value = ft_strdup(current->value);
	free(current->value);
	current->value = getcwd(NULL, 0);
}

void	cd_missing_dir(char **tokens)
{
	ft_putstr_fd("bash: cd: ", 2);
	ft_putstr_fd(tokens[1], 2);
	ft_putendl_fd(": Inexistent file or directory", 2);
	g_global.exit_status = 1;
}
