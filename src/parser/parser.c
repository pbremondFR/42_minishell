/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 21:42:14 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/11 17:42:48 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Counts the number of tokens until a separator is found, thus giving us the
// number of args, minus program name.
int	ms_prs_argv_size(t_list *tok_stream)
{
	int			count;
	t_lex_tok	*token;

	count = 0;
	while (tok_stream)
	{
		token = tok_stream->content;
		if (ms_prs_is_sep(token->type))
			break ;
		count++;
		tok_stream = tok_stream->next;
	}
	return (count);
}

// Allocates the 2D array for argv and fills it with ft_strdup.
// NOTE: name useless for now. Maybe if want to replace cmd name w/ full path?
char	**ms_prs_create_argv(t_list *tok_stream, char *name)
{
	int			count;
	int			i;
	char		**argv;
	t_lex_tok	*token;

	(void)name;
	count = ms_prs_argv_size(tok_stream);
	argv = malloc(sizeof(char *) * (count + 2));
	if (argv == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_CMD_LST | F_GLOBAL, false);
	i = 0;
	while (i < count)
	{
		token = tok_stream->content;
		argv[i++] = ms_prs_parse_word(token->value);
		tok_stream = tok_stream->next;
	}
	argv[i] = NULL;
	return (argv);
}

// NOTE: Don't return NULL if cmd is a builtin
char	*ms_prs_get_bin_path(t_prs_cmd *cmd, char *prgm_name)
{
	(void)cmd;
	if (prgm_name == NULL)
		return (NULL);
	if (prgm_name[0] == '.' || prgm_name[0] == '/')
		return (ft_strdup(prgm_name));
	else
		return (find_exec(prgm_name));
}

// Returns a chained list, each node is a program and its args
t_list	*ms_parser(t_list *tok_strm)
{
	t_prs_cmd	*cmd;
	t_list		*cmd_list;
	t_list		*cmd_node;
	int			id;

	cmd_list = NULL;
	id = 0;
	while (tok_strm)
	{
		cmd = ms_prs_create_cmd(tok_strm->content, tok_strm, id++);
		cmd_node = ft_lstnew(cmd);
		ft_lstadd_back(&cmd_list, cmd_node);
		if (ms_prs_is_sep(((t_lex_tok *)tok_strm->content)->type))
			tok_strm = tok_strm->next;
		else
		{
			while (tok_strm
				&& !ms_prs_is_sep(((t_lex_tok *)tok_strm->content)->type))
				tok_strm = tok_strm->next;
		}
	}
	return (cmd_list);
}

// Returns 1 if given token type is a separator, 0 otherwise.
int	ms_prs_is_sep(t_tok_typ type)
{
	if (type == T_PIPE || type == T_SEMI || type == T_SEP)
		return (1);
	else
		return (0);
}
