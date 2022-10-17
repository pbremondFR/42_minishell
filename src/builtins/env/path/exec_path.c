/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 10:06:14 by jrathelo          #+#    #+#             */
/*   Updated: 2022/01/09 12:40:52 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Executes a command.
// Unsure if we are using errno or not, if we are its an idea to set it to 2 if
// access does not find the executable file.
int	exec_command(char *path, char *args[])
{
	char	**envp;
	int		ret;

	envp = convert_envp();
	if (access(path, X_OK))
	{
		ret = execve(path, args, envp);
		free_array(envp);
		return (ret);
	}
	free_array(envp);
	return (-1);
}

// Convertes the global variable into a temperary variable to pass
// into execve.
char	**convert_envp(void)
{
	size_t	s;
	size_t	i;
	char	**temp;
	t_list	*next;

	s = ft_lstsize(g_ms->envp);
	temp = ft_calloc(s + 1, sizeof(char *));
	next = g_ms->envp;
	i = 0;
	while (next->next != NULL)
	{
		temp[i] = ft_strdup(next->content);
		next = next->next;
		i++;
	}
	temp[i] = ft_strdup(next->content);
	next = next->next;
	temp[++i] = NULL;
	return (temp);
}
