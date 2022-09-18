/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cleticia <cleticia@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:18:05 by lfranca-          #+#    #+#             */
/*   Updated: 2022/05/24 04:35:06 by cleticia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_global	g_global;

static char	*mm_init_prompt(void)
{
	char	*to_trim;
	char	*prompt;

	prompt = ft_strdup("$> ");
	se_set_main_signals();
	to_trim = readline(prompt);
	free(prompt);
	return (to_trim);
}

static int	is_invalid_empty_input(char *to_trim)
{
	if (!to_trim || ft_strlen(to_trim) == 0)
	{
		if (!to_trim)
			ma_clear_ctrld();
		else
			free(to_trim);
		return (1);
	}
	return (0);
}

void	mm_main_loop(int ret)
{
	char	*to_trim;
	char	*cmdline;

	while (1)
	{
		to_trim = mm_init_prompt();
		if (is_invalid_empty_input(to_trim) == 1)
			continue ;
		cmdline = ft_strtrim(to_trim, " ");
		free(to_trim);
		if (ft_strlen(cmdline) == 0)
		{
			free(cmdline);
			continue ;
		}
		add_history(cmdline);
		ret = pc_management_parse(cmdline);
		free(cmdline);
		ma_decide_exec(ret);
	}
}

void	mm_list_environ(char **environ)
{
	t_variables	*tmp;
	int			counter;
	char		**content_env;

	counter = 0;
	while (environ[counter])
	{
		content_env = ft_split(environ[counter], '=');
		if (g_global.head_env == NULL)
			g_global.head_env = el_var_node(content_env);
		else
		{
			tmp = g_global.head_env;
			while (tmp->next != NULL)
				tmp = tmp->next;
			tmp->next = el_var_node(content_env);
		}
		er_free_matrix(content_env);
		free(content_env);
		counter++;
	}
}

int	main(int argc, char **argv, char **environ)
{
	int		ret;

	ret = 0;
	g_global.head_env = NULL;
	g_global.head_local = NULL;
	if (argc > 1 && argv)
	{
		ft_putstr_fd("error: Run only the program name!\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	mm_list_environ(environ);
	mm_main_loop(ret);
	return (0);
}
