/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 10:58:12 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/08 17:50:15 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Looks for the executable, returns the path where it found the executabe
// Returns NULL if nothing was found.
char	*find_exec(char *command)
{
	char	**paths;
	char	*temp;
	size_t	i;

	paths = get_paths();
	if (paths == NULL)
		return (ft_strdup(command));
	i = 0;
	while (paths[i] != NULL)
	{
		temp = ft_strjoin(paths[i], command);
		if (access(temp, X_OK) != -1)
		{
			free_array(paths);
			return (temp);
		}
		free(temp);
		i++;
	}
	free_array(paths);
	return (NULL);
}

// Returns a table contatining all the PATHS.
char	**get_paths(void)
{
	char	**ret;
	char	*temp;
	size_t	i;

	temp = ms_find_env("PATH");
	if (temp == NULL)
		return (NULL);
	ret = ft_split(temp, ':');
	free(temp);
	i = 0;
	while (ret[i] != NULL)
	{
		temp = ft_strjoin(ret[i], "/");
		free(ret[i]);
		ret[i] = temp;
		i++;
	}
	return (ret);
}

// Function to free arrays of strings, need due to ft_split.
void	free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
