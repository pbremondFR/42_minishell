/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mario.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 13:29:38 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/09 16:02:05 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

int	ms_find_last_stdout_in_chain(t_prs_cmd *cmd)
{
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	while (cmd->redir_array[i] != NULL)
	{
		if (cmd->redir_array[i]->type == T_DGREAT
			|| cmd->redir_array[i]->type == T_SGREAT)
			j = i;
		i++;
	}
	return (j);
}

void	ms_prepare_execution(t_prs_cmd **cmds, size_t i, int type)
{
	if (type & C_ENTER || type & C_BRIDGE)
		dup2(cmds[i - 1]->pipe_fd[0], STDIN_FILENO);
	if (type & C_EXIT || type & C_BRIDGE)
		dup2(cmds[i + 1]->pipe_fd[1], STDOUT_FILENO);
	if (type & R_IN)
		dup2(cmds[i]->fd_in, STDIN_FILENO);
	if (type & R_HERE)
		dup2(cmds[i]->pipe_fd[0], STDIN_FILENO);
	if (type & R_OUT_D || type & R_OUT_S)
		dup2(cmds[i]->fd_out, STDOUT_FILENO);
	ms_close_pipes(cmds);
	ms_exec_prgm(cmds[i]->bin_path, cmds[i]->argv);
}

void	ms_prepare_bulitin_exec(t_prs_cmd **cmds, size_t i, int type)
{
	int	stdout_temp;

	stdout_temp = dup(STDOUT_FILENO);
	if (type & C_ENTER || type & C_BRIDGE)
		close(cmds[i - 1]->pipe_fd[0]);
	if (type & R_IN || type & R_HERE)
		close(cmds[i]->fd_in);
	if (type & C_EXIT || type & C_BRIDGE)
		dup2(cmds[i + 1]->pipe_fd[1], STDOUT_FILENO);
	if (type & R_OUT_D || type & R_OUT_S)
		dup2(cmds[i]->fd_out, STDOUT_FILENO);
	ms_exec_builtin(cmds[i]);
	dup2(stdout_temp, STDOUT_FILENO);
}

int	ms_check_if_is_stdout(t_prs_cmd **cmds, size_t i, int j)
{
	return (cmds[i]->redir_array[j]->type == T_SGREAT
		|| cmds[i]->redir_array[j]->type == T_DGREAT);
}

void	ms_redirect_stdout(t_prs_cmd **cmds, size_t i)
{
	int	fd;
	int	j;
	int	t;

	j = 0;
	fd = -1;
	t = ms_find_last_stdout_in_chain(cmds[i]);
	while (cmds[i]->redir_array[j] != NULL && j < t)
	{
		if (cmds[i]->redir_array[j]->type == T_SGREAT)
			fd = open(cmds[i]->redir_array[j]->value, O_WRONLY | O_CREAT
					| O_TRUNC, 0644);
		else if (cmds[i]->redir_array[j]->type == T_DGREAT)
			fd = open(cmds[i]->redir_array[j]->value, O_WRONLY | O_CREAT
					| O_APPEND, 0644);
		if (fd == -1 && ms_check_if_is_stdout(cmds, i, j))
			ms_perror(cmds[i]->redir_array[j]->value, strerror(errno));
		else if (fd >= 0 && ms_check_if_is_stdout(cmds, i, j))
		{
			write(fd, "", 0);
			close(fd);
		}
		j++;
	}
	ms_stdout_redirect_handle(cmds, t, i);
}
