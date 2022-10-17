/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wario.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrathelo <jrathelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 10:41:36 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/09 13:41:26 by jrathelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void	ms_setup_redirects(t_prs_cmd **cmds, size_t i, int type)
{
	if ((type & R_IN) || (type & R_HERE))
		ms_redirect_stdin(cmds, i);
	if ((type & R_OUT_S) || (type & R_OUT_D))
		ms_redirect_stdout(cmds, i);
}

void	ms_redirects_only(t_prs_cmd *cmds)
{
	size_t	i;
	int		fd;

	i = 0;
	while (cmds->redir_array[i])
	{
		if (cmds->redir_array[i]->type == T_SGREAT
			|| cmds->redir_array[i]->type == T_DGREAT)
		{
			fd = open(cmds->redir_array[i]->value, O_CREAT, 0644);
			if (fd == -1)
				ms_perror(cmds->redir_array[i]->value, strerror(errno));
			close(fd);
		}
		else if (cmds->redir_array[i]->type == T_SLESS)
		{
			fd = open(cmds->redir_array[i]->value, O_RDONLY, 0644);
			if (fd == -1)
				ms_perror(cmds->redir_array[i]->value, strerror(errno));
			close(fd);
		}
		else if (cmds->redir_array[i]->type == T_DLESS)
			ms_faker_heredoc(cmds->redir_array[i]->value);
		i++;
	}
}

void	ms_find_and_run_commands(t_prs_cmd **cmds)
{
	size_t	i;

	i = 0;
	while (cmds[i] != NULL && cmds[1] != NULL)
	{
		if (cmds[i]->type == T_CMD)
		{
			cmds[i]->package = 0;
			cmds[i]->package |= ms_cmd_has_redirects(cmds[i]);
			if (cmds[i]->package == C_SOLO)
				cmds[i]->package = 0;
			cmds[i]->package |= ms_cmd_pipe_status(cmds, i, cmds[i]->package);
			ms_setup_redirects(cmds, i, cmds[i]->package);
		}
		if (cmds[i]->type == T_BLANK)
			ms_redirects_only(cmds[i]);
		i++;
	}
	i = 0;
	while (cmds[i] != NULL && cmds[1] != NULL)
	{
		if (cmds[i]->type == T_CMD)
			ms_fork_plumber(cmds, i, cmds[i]->package);
		i++;
	}
}

int	ms_cmd_has_redirects(t_prs_cmd *cmd)
{
	size_t	i;
	int		temp;

	i = 0;
	temp = 0;
	if (cmd->redir_array == NULL)
		return (C_SOLO);
	while (cmd->redir_array[i] != NULL)
	{
		if (!(temp & R_OUT_D) && cmd->redir_array[i]->type == T_DGREAT)
			temp |= R_OUT_D;
		else if (!(temp & R_OUT_S) && cmd->redir_array[i]->type == T_SGREAT)
			temp |= R_OUT_S;
		else if (!(temp & R_IN) && cmd->redir_array[i]->type == T_SLESS)
			temp |= R_IN;
		else if (!(temp & R_HERE) && cmd->redir_array[i]->type == T_DLESS)
			temp |= R_HERE;
		i++;
	}
	return (temp);
}

int	ms_cmd_pipe_status(t_prs_cmd **cmds, size_t i, int package)
{
	if (!(package & R_OUT_D) && !(package & R_OUT_S))
		if (cmds[i + 1] != NULL)
			package |= C_EXIT;
	if (!(package & R_IN) && !(package & R_HERE))
		if (i != 0)
			package |= C_ENTER;
	if ((package & C_ENTER) && (package & C_EXIT))
	{
		package ^= (C_EXIT | C_ENTER);
		package |= C_BRIDGE;
	}
	return (package);
}
