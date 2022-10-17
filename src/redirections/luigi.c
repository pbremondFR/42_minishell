/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   luigi.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrathelo <jrathelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:23:15 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/09 12:40:41 by jrathelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int	ms_find_last_stdin_in_chain(t_prs_cmd *cmd)
{
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	while (cmd->redir_array[i] != NULL)
	{
		if (cmd->redir_array[i]->type == T_SLESS
			|| cmd->redir_array[i]->type == T_DLESS)
			j = i;
		i++;
	}
	return (j);
}

void	ms_redirect_stdin(t_prs_cmd **cmds, int i)
{
	int		j;
	int		t;
	int		fd;

	j = 0;
	t = ms_find_last_stdin_in_chain(cmds[i]);
	while (cmds[i]->redir_array[j] != NULL && j < t)
	{
		if (cmds[i]->redir_array[j]->type == T_SLESS)
		{
			fd = open(cmds[i]->redir_array[j]->value, O_RDONLY);
			if (fd == -1)
				ms_perror(cmds[i]->redir_array[j]->value, strerror(errno));
			else
				close(fd);
		}
		else if (cmds[i]->redir_array[j]->type == T_DLESS)
			ms_faker_heredoc(cmds[i]->redir_array[j]->value);
		j++;
	}
	if (cmds[i]->redir_array[j]->type == T_SLESS)
		ms_stdin_redirect_handle(cmds, t, i);
	if (cmds[i]->redir_array[j]->type == T_DLESS)
		ms_heredoc_handle(cmds, t, i);
}
