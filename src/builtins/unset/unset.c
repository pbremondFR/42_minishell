/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 14:04:27 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/11 18:07:41 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// FIXME! Multiple args & keys without values
// export y=p=o
int	ms_unset(char *t)
{
	t_list	*crnt;
	t_list	*prev;
	char	*target;

	if (ms_export_is_format_valid(t, "unset") == false)
		return (EXIT_FAILURE);
	crnt = g_ms->envp;
	prev = NULL;
	target = ft_strjoin(t, "=");
	while (crnt)
	{
		if (ft_strncmp(crnt->content, target, ft_strlen(target)) == 0)
		{
			if (prev)
				prev->next = crnt->next;
			else
				g_ms->envp = crnt->next;
			ft_lstdelone(crnt, &free);
			break ;
		}
		prev = crnt;
		crnt = crnt->next;
	}
	free(target);
	return (EXIT_SUCCESS);
}
