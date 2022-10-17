/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plumber.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 13:57:04 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/10 19:43:09 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

static void	ms_init_pipes(t_prs_cmd **cmds)
{
	size_t	i;

	i = 0;
	while (cmds[i] != NULL)
	{
		pipe(cmds[i]->pipe_fd);
		if (cmds[i]->type == T_CMD)
		{
			cmds[i]->fd_in = -1;
			cmds[i]->fd_out = -1;
		}
		i++;
	}
}

void	ms_close_pipes(t_prs_cmd **cmds)
{
	size_t	i;

	i = 0;
	while (cmds[i] != NULL)
	{
		if (cmds[i]->pipe_fd[0] != -1)
			close(cmds[i]->pipe_fd[0]);
		if (cmds[i]->pipe_fd[1] != -1)
			close(cmds[i]->pipe_fd[1]);
		i++;
	}
}

static void	ms_wait_for_processes(t_prs_cmd **cmds)
{
	size_t	i;
	int		stat_loc;

	i = 0;
	stat_loc = 0;
	while (cmds[i] != NULL)
	{
		if (cmds[i]->type == T_CMD && cmds[i]->pid != -1)
		{
			waitpid(cmds[i]->pid, &stat_loc, 0);
			if (WIFEXITED(stat_loc))
				cmds[i]->code = WEXITSTATUS(stat_loc);
			else
				cmds[i]->code = 1;
		}
		i++;
	}
}

// NOTE: Leak on fork() call is not from us and seems to be a false positive
// or expected behaviour.
void	ms_fork_plumber(t_prs_cmd **cmds, size_t i, int type)
{
	int		fd;

	fd = -1;
	if (cmds[i]->bin_path != NULL)
		fd = open(cmds[i]->bin_path, O_WRONLY);
	if (fd != -1)
		close(fd);
	if (ms_is_builtin(cmds[i]))
	{
		ms_prepare_bulitin_exec(cmds, i, type);
		cmds[i]->pid = -1;
	}
	else if (cmds[i]->bin_path == NULL || (fd == -1 && errno == EISDIR))
	{
		ms_error_with_cmd(cmds, i, errno);
		return ;
	}
	else
		cmds[i]->pid = fork();
	if (cmds[i]->pid == 0)
		ms_prepare_execution(cmds, i, type);
	if (cmds[i]->fd_in != -1)
		close(cmds[i]->fd_in);
	if (cmds[i]->fd_out != -1)
		close(cmds[i]->fd_out);
}

int	ms_handle_redirections(t_prs_cmd **cmds, struct termios term)
{
	ms_init_pipes(cmds);
	if (cmds[0] == NULL)
		return (0);
	tcgetattr(0, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
	signal(SIGINT, &ms_sig_forked_handler);
	if (cmds[1] == NULL)
	{
		if (ft_strncmp(cmds[0]->token->value, "exit", 5) == 0)
			ms_exec_exit(cmds[0]);
		ms_setup_redirects(cmds, 0, ms_cmd_has_redirects(cmds[0]));
		ms_fork_plumber(cmds, 0, ms_cmd_has_redirects(cmds[0]));
	}
	ms_find_and_run_commands(cmds);
	ms_close_pipes(cmds);
	ms_wait_for_processes(cmds);
	tcgetattr(0, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
	ms_init_signals();
	return (ms_get_return_code(cmds));
}
