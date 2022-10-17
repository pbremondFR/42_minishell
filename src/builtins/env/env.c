/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 11:45:30 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/09 14:56:48 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int	ms_env_checker(char *s)
{
	size_t	i;

	i = 1;
	if (ft_isdigit(s[0]))
		return (-1);
	while (s[i] != '=' && s[i] != '\x00')
	{
		if (!ft_isprint(s[i]))
			return (-1);
		i++;
	}
	if (s[i] == '\x00')
		return (-1);
	return (i);
}

// Bulitin env
// Prints out all the environment variables
int	env(void)
{
	t_list	*envp_lst;

	envp_lst = g_ms->envp;
	while (envp_lst)
	{
		if (ms_env_checker((char *)envp_lst->content) != -1)
			printf("%s\n", (char *)envp_lst->content);
		envp_lst = envp_lst->next;
	}
	return (EXIT_SUCCESS);
}
