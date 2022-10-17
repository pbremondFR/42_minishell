/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrathelo <jrathelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:19:57 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/09 12:30:02 by jrathelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

int	error_out(int pipe[2], t_prs_cmd *cmd)
{
	ms_perror("Error", strerror(errno));
	cmd->pid = -1;
	cmd->code = 1;
	close(pipe[1]);
	close(pipe[0]);
	return (1);
}

// Preforms a redirection for >> and >. Will also create a file if it 
// doesn't exist
int	ms_stdout_redirect_handle(t_prs_cmd **cmds, int target, int i)
{
	int		fd;

	fd = -1;
	if (cmds[i]->redir_array[target]->type == T_SGREAT)
		fd = open(cmds[i]->redir_array[target]->value, O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
	else if (cmds[i]->redir_array[target]->type == T_DGREAT)
		fd = open(cmds[i]->redir_array[target]->value, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
	if (fd == -1)
		return (error_out(cmds[i]->pipe_fd, cmds[i]));
	cmds[i]->fd_out = fd;
	return (0);
}

// Preforms a redirection for <. Errors if file does not exist
int	ms_stdin_redirect_handle(t_prs_cmd **cmds, int target, int i)
{
	int		fd;

	fd = open(cmds[i]->redir_array[target]->value, O_RDONLY);
	if (fd == -1)
		return (error_out(cmds[i]->pipe_fd, cmds[i]));
	cmds[i]->fd_in = fd;
	return (fd);
}

char	*ms_heredoc_logic(t_prs_cmd **cmds, int target, int i)
{
	char	*line;
	char	*multi_lines;

	line = readline("> ");
	multi_lines = NULL;
	while (ft_strncmp(line, cmds[i]->redir_array[target]->value,
			ft_strlen(cmds[i]->redir_array[target]->value) + 1) != 0)
	{
		if (multi_lines == NULL)
		{
			line = ft_stradd2(line, "\n", 1);
			multi_lines = line;
		}
		else
		{
			multi_lines = ft_stradd2(multi_lines, line, 3);
			multi_lines = ft_stradd2(multi_lines, "\n", 1);
		}
		line = readline("> ");
	}
	free(line);
	return (multi_lines);
}

void	ms_heredoc_handle(t_prs_cmd **cmds, int target, int i)
{
	char	*line;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &ms_sig_faker_handler);
		signal(SIGQUIT, &ms_sig_faker_handler);
		line = ms_heredoc_logic(cmds, target, i);
		if (line == NULL)
			write(cmds[i]->pipe_fd[1], "\x00", 1);
		else
		{
			write(cmds[i]->pipe_fd[1], line, ft_strlen(line));
			free(line);
		}
		ms_close_pipes(cmds);
		ms_exit(0, F_ALL, false);
	}
	waitpid(pid, NULL, 0);
}
