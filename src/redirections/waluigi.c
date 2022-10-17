/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waluigi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 15:07:19 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/10 18:27:31 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	ms_error_with_cmd(t_prs_cmd **cmds, int i, int enumb)
{
	if (cmds[i]->bin_path == NULL)
	{
		ms_perror(cmds[i]->argv[0], "command not found");
		cmds[i]->pid = -1;
		cmds[i]->code = 127;
	}
	else
	{
		ms_perror(cmds[i]->argv[0], strerror(enumb));
		cmds[i]->pid = -1;
		cmds[i]->code = 126;
	}	
	return ;
}

int	ms_get_return_code(t_prs_cmd **cmds)
{
	size_t	i;
	int		code;

	i = 0;
	code = 0;
	while (cmds[i] != NULL)
	{
		if (cmds[i]->type == T_CMD)
			code = cmds[i]->code;
		i++;
	}
	return (code);
}

void	ms_sig_faker_handler(int sig)
{
	(void) sig;
	ms_exit(0, F_ALL, false);
}

void	ms_faker_heredoc(char *value)
{
	char	*line;
	pid_t	pid;

	signal(SIGINT, &ms_sig_forked_handler);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &ms_sig_faker_handler);
		signal(SIGQUIT, &ms_sig_faker_handler);
		line = readline("> ");
		while (ft_strncmp(line, value, ft_strlen(value) + 1) != 0)
		{
			free(line);
			line = readline("> ");
		}
		free(line);
		ms_exit(0, F_ALL, false);
	}
	waitpid(pid, NULL, 0);
	ms_init_signals();
}

void	ms_only_redirects(t_list *rl)
{
	int	fd;

	while (rl != NULL)
	{
		if (((t_lex_tok *)rl->content)->type == T_SGREAT
			|| ((t_lex_tok *)rl->content)->type == T_DGREAT)
		{
			fd = open(((t_lex_tok *)rl->content)->value, O_CREAT, 0644);
			if (fd == -1)
				ms_perror(((t_lex_tok *)rl->content)->value, strerror(errno));
			close(fd);
		}
		else if (((t_lex_tok *)rl->content)->type == T_SLESS)
		{
			fd = open(((t_lex_tok *)rl->content)->value, O_RDONLY, 0644);
			if (fd == -1)
				ms_perror(((t_lex_tok *)rl->content)->value, strerror(errno));
			close(fd);
		}
		else if (((t_lex_tok *)rl->content)->type == T_DLESS)
			ms_faker_heredoc(((t_lex_tok *)rl->content)->value);
		rl = rl->next;
	}
}
