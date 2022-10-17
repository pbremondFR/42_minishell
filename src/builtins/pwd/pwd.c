/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 20:00:07 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/09 15:41:32 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Prints out path to current working dir. Is equivalent to pwd(1) or pwd(1) -L
int	ft_pwd(void)
{
	char	*cwd;

	cwd = ms_find_env("PWD");
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		if (update_pwd() != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		cwd = ms_find_env("PWD");
		printf("%s\n", cwd);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}
