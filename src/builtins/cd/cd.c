/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 17:12:53 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/11 17:41:24 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

#define RELATIVE	0
#define ABSOLUTE	1

#define NO_FREE		0
#define FREE_OLD	0b1
#define FREE_NEW	0b10
#define FREE_HOME	0b100

// NOTE: getenv() returns the address of string in *envp[]

// Frees pointers corresponding to given OR'ed flags (e.g. FREE_OLD | FREE_HOME)
static int	exit_fn(struct s_cd_data cd, int exit_code, int flags)
{
	if (flags & FREE_OLD)
		free(cd.old_cwd);
	if (flags & FREE_NEW)
		free(cd.new_cwd);
	if (flags & FREE_HOME)
		free(cd.home);
	if (exit_code != EXIT_SUCCESS && errno != 0)
		perror("minishell: cd");
	return (exit_code);
}

void	update_oldpwd(const char *value)
{
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", value);
	ms_export(oldpwd);
	free(oldpwd);
}

int	update_pwd(void)
{
	char		*pwd;
	char		*current_dir;

	current_dir = getcwd(NULL, 0);
	if (current_dir == NULL)
		return (EXIT_FAILURE);
	pwd = ft_strjoin("PWD=", current_dir);
	ms_export(pwd);
	free(current_dir);
	free(pwd);
	return (EXIT_SUCCESS);
}

// Expands ~ as $HOME if found. If path is NULL, chdir to $HOME. Updates
// $OLDPWD and $PWD as needed.
int	ft_cd(const char *path)
{
	t_cd_dat	cd;

	cd.old_cwd = ms_find_env("PWD");
	cd.home = ms_find_env("HOME");
	if (path == NULL)
	{
		if (cd.home)
			cd.new_cwd = ft_strjoin("/", cd.home);
		else
		{
			ms_perror("cd", "HOME not set");
			return (1);
		}
	}
	else if (path[0] == '~' && cd.home)
		cd.new_cwd = ft_strjoin(cd.home, path + 1);
	else
		cd.new_cwd = ft_strdup(path);
	if (chdir(cd.new_cwd) != EXIT_SUCCESS)
		return (exit_fn(cd, EXIT_FAILURE, FREE_OLD | FREE_NEW | FREE_HOME));
	update_oldpwd(cd.old_cwd);
	if (update_pwd() != EXIT_SUCCESS)
		return (exit_fn(cd, EXIT_FAILURE, F_ALL));
	return (exit_fn(cd, EXIT_SUCCESS, FREE_OLD | FREE_NEW | FREE_HOME));
}

int	ft_exec_cd(const char **argv)
{
	if (ms_array_size(argv) > 2)
	{
		ms_perror("cd", "too many parameters");
		return (EXIT_FAILURE);
	}
	return (ft_cd(argv[1]));
}
