/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfranca- <lfranca-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/22 22:54:41 by cleticia          #+#    #+#             */
/*   Updated: 2022/06/02 12:30:22 by lfranca-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	cd_home(char **tokens)
{
	t_variables	*home_item;
	char		*path;

	home_item = el_search_var(g_global.head_env, "HOME");
	path = home_item->value;
	if (tokens[1] != NULL && tokens[1][1] == '~')
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(tokens[1], 2);
		ft_putendl_fd(": Inexistent file or directory", 2);
		g_global.exit_status = 1;
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(tokens[1], 2);
		ft_putendl_fd(": Inexistent file or directory", 2);
		return (1);
	}
	cd_old_pwd();
	return (EXIT_SUCCESS);
}

int	cd_parent_dir(char **tokens)
{
	char	*current_dir;
	int		reverse_index;
	char	*parent_dir;

	current_dir = getcwd(NULL, 0);
	reverse_index = ft_strlen(current_dir);
	while (current_dir[reverse_index] != '/')
		reverse_index--;
	if (current_dir[reverse_index] == '/')
		parent_dir = ft_substr(current_dir, 0, reverse_index);
	free(current_dir);
	if (chdir(parent_dir) == -1)
	{
		free(parent_dir);
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(tokens[1], 2);
		ft_putendl_fd(": Inexistent file or directory", 2);
		return (127);
	}
	cd_old_pwd();
	free(parent_dir);
	return (EXIT_SUCCESS);
}

static int	cd_absolute_path(char **tokens)
{
	char		*path;
	t_variables	*home_item;

	if (tokens[1][0] == '~')
	{
		home_item = el_search_var(g_global.head_env, "HOME");
		path = ft_strjoin(home_item->value, ft_strchr(tokens[1], '/'));
	}
	else if (tokens[1][0] == '/')
		path = ft_strdup(tokens[1]);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("bash: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Inexistent file or directory", 2);
		free(path);
		g_global.exit_status = 1;
		return (1);
	}
	free(path);
	cd_old_pwd();
	g_global.exit_status = 0;
	return (EXIT_SUCCESS);
}

static void	cd_relative_path(char **tokens, struct stat *s)
{
	char		*path;
	char		*aux;

	path = NULL;
	if (s->st_mode & S_IFDIR)
	{
		aux = getcwd(NULL, 0);
		path = ft_strjoin(aux, "/");
		free(aux);
		aux = path;
		path = ft_strjoin(aux, tokens[1]);
		free(aux);
	}
	if (chdir(path) == -1)
		cd_missing_dir(tokens);
	else
	{
		cd_old_pwd();
		g_global.exit_status = 0;
	}
	if (path)
		free(path);
}

void	ft_cd(char **tks)
{
	struct stat	s;
	int			ret;

	ret = 0;
	if (cd_error_excess_args(tks) == 1)
		return ;
	if (tks[1] != NULL && tks[1][0] == '/')
		cd_absolute_path(tks);
	else if (tks[1] != NULL && tks[1][0] != '~' && tks[1][0] != '/')
	{
		if (stat(tks[1], &s) == 0)
			cd_relative_path(tks, &s);
		else
			cd_missing_dir(tks);
	}
	else if (tks[1] == NULL || (tks[1] != NULL && tks[1][0] == '~'))
	{
		if (tks[1] != NULL && (tks[1][1] == '/' && (ft_strlen(tks[1]) > 2)))
			ret = cd_absolute_path(tks);
		else
			ret = cd_home(tks);
		g_global.exit_status = ret;
	}
}
