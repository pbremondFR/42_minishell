/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/28 22:25:34 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 16:53:59 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

struct termios	ms_init_termios_struct(void)
{
	struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(0, TCSANOW, &term);
	return (term);
}

void	ms_clear_alot_of_lists(void)
{
	if (g_ms->tok_list != NULL)
		ft_lstclear(&g_ms->tok_list, &ms_free_token);
	if (g_ms->cmd_list != NULL)
		ft_lstclear(&g_ms->cmd_list, &ms_free_cmd);
	if (g_ms->cmd_array != NULL)
	{
		free(g_ms->cmd_array);
		g_ms->cmd_array = NULL;
	}
	if (g_ms->redir_list != NULL)
		ft_lstclear(&g_ms->redir_list, &ms_free_token);
}

static struct termios	init_vars(int argc, char const *argv[],
	char const *envp[])
{
	(void)argc;
	(void)argv;
	ms_init_global_var(envp);
	ms_init_signals();
	return (ms_init_termios_struct());
}

	// debug_print_tok_list(g_ms->redir_list, "redir_list");
	// debug_print_tok_list(g_ms->tok_list, "tok_list");
static void	_ms_parse_cycle(char *line)
{
	g_ms->tok_list = ms_lexer(line, ft_strlen(line));
	free(line);
	g_ms->redir_list = ms_preparser(&g_ms->tok_list);
	ms_token_padding(&g_ms->tok_list, &g_ms->redir_list);
	g_ms->cmd_list = ms_parser(g_ms->tok_list);
}

			// ms_tests(g_ms->tok_list, g_ms->cmd_list, g_ms->cmd_array);
int	main(int argc, char const *argv[], char const *envp[])
{
	char			*line;
	struct termios	term;

	term = init_vars(argc, argv, envp);
	while (1)
	{
		line = ms_prompt();
		if (line == NULL)
			ms_exit(errno, F_ENVP | F_GLOBAL, true);
		ms_historian(line);
		_ms_parse_cycle(line);
		if (g_ms->cmd_list == NULL)
			ms_only_redirects(g_ms->redir_list);
		else
		{
			g_ms->cmd_array = ms_prs_lst_2darray(g_ms->cmd_list);
			ms_prs_map_redirs_to_cmd(g_ms->redir_list, g_ms->cmd_array);
			g_ms->last_code = ms_handle_redirections(g_ms->cmd_array, term);
		}
		ms_clear_alot_of_lists();
	}
	free_global_var(F_ALL);
}
