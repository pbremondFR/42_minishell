/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 08:06:59 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/04 11:42:41 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Once again something that would have been fine in a ternary...
int	is_sgreat_dgreat(t_tok_typ type)
{
	if (type == T_SGREAT || type == T_DGREAT)
		return (1);
	return (0);
}

// Once again something that would have been fine in a ternary...
int	is_sless_dless(t_tok_typ type)
{
	if (type == T_SLESS || type == T_DLESS)
		return (1);
	return (0);
}

void	ms_free_cmd(void *command)
{
	size_t		i;
	t_prs_cmd	*cmd;

	cmd = command;
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
			i++;
		cmd->argv = ft_split_free(cmd->argv, i);
	}
	ft_strdel(&cmd->bin_path);
	free(cmd->redir_array);
	free(cmd);
}

t_prs_cmd	*ms_prs_create_cmd(t_lex_tok *token, t_list *tok_strm, int id)
{
	t_prs_cmd	*cmd;

	cmd = malloc(sizeof(t_prs_cmd));
	if (cmd == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_GLOBAL, false);
	cmd->token = tok_strm->content;
	cmd->id = id;
	if (token->type == T_STRING)
		cmd->type = T_CMD;
	else
		cmd->type = token->type;
	cmd->argv = NULL;
	cmd->bin_path = NULL;
	cmd->redir_array = NULL;
	if (is_tok_lex_sep(cmd->type) == 0)
	{
		cmd->argv = ms_prs_create_argv(tok_strm, token->value);
		cmd->bin_path = ms_prs_get_bin_path(cmd, cmd->argv[0]);
	}
	return (cmd);
}

// Creates a 2d array containing all parser commands.
// WARNING: ONLY free the array itself, and not every command.
// For simplicity's sake, the array contains pointers to the original
// cmd nodes, which are free'd when clearing the cmd_list.
t_prs_cmd	**ms_prs_lst_2darray(t_list *cmd_lst)
{
	t_prs_cmd	*cmd;
	t_prs_cmd	**cmd_array;
	int			lst_siz;
	int			i;

	lst_siz = ft_lstsize(cmd_lst);
	cmd_array = malloc((lst_siz + 1) * (sizeof(t_prs_cmd *)));
	if (cmd_array == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_CMD_LST | F_GLOBAL, false);
	i = 0;
	while (cmd_lst)
	{
		cmd = cmd_lst->content;
		cmd_array[i++] = cmd;
		cmd_lst = cmd_lst->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}
