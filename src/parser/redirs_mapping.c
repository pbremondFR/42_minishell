/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_mapping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 13:51:47 by jrathelo          #+#    #+#             */
/*   Updated: 2022/02/16 14:50:34 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Returns the number of t_list nodes until a token of type T_SEP.
static size_t	size_until_sep(t_list *redir_lst)
{
	t_lex_tok	*tok;
	size_t		i;

	i = 0;
	tok = redir_lst->content;
	while (redir_lst && tok->type != T_SEP)
	{
		redir_lst = redir_lst->next;
		if (redir_lst)
			tok = redir_lst->content;
		i++;
	}
	return (i);
}

// Maps the command's redirections into a 2D array. Redirections are separated
// by a T_SEP token.
static t_lex_tok	**map_redirs_for_command(t_list *redir_list)
{
	t_lex_tok	**redir_array;
	t_lex_tok	*tok;
	size_t		array_size;
	size_t		i;

	array_size = size_until_sep(redir_list);
	if (array_size == 0)
		return (NULL);
	redir_array = malloc((array_size + 1) * sizeof(t_lex_tok *));
	if (redir_array == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_CMD_LST | F_GLOBAL, false);
	i = 0;
	while (redir_list && i < array_size)
	{
		tok = redir_list->content;
		redir_array[i++] = tok;
		redir_list = redir_list->next;
	}
	redir_array[i] = NULL;
	return (redir_array);
}

// Fills in the redir_array field in appropriate t_prs_cmd.
void	ms_prs_map_redirs_to_cmd(t_list *redir_list, t_prs_cmd **cmds)
{
	size_t	i;

	i = 0;
	while (redir_list)
	{
		cmds[i]->redir_array = map_redirs_for_command(redir_list);
		while (redir_list
			&& ((t_lex_tok *)(redir_list->content))->type != T_SEP)
			redir_list = redir_list->next;
		i += 2;
		if (redir_list)
			redir_list = redir_list->next;
	}
}

/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/* void	ms_allocate_redirect_array(t_prs_cmd **cmds, t_list *redirections)
{
	t_list	*next;
	size_t	pos;
	size_t	size;

	pos = 0;
	size = 0;
	next = redirections;
	while (next->next != NULL)
	{
		if (((t_lex_tok *)next->content)->value == NULL)
		{
			cmds[pos]->redir_array = ft_calloc(size + 1, sizeof(t_lex_tok *));
			size = 0;
			pos += 2;
		}
		else
			size ++;
		next = next->next;
	}
	if (((t_lex_tok *)next->content)->value == NULL)
	{
		cmds[pos]->redir_array = ft_calloc(size + 1, sizeof(t_lex_tok *));
		cmds[pos + 2]->redir_array = ft_calloc(1, sizeof(t_lex_tok *));
	}
	else
	{
		cmds[pos]->redir_array = ft_calloc(size + 2, sizeof(t_lex_tok *));
	}
}

void	ms_fill_redirect_array(t_prs_cmd **cmds, t_list *redirections)
{
	t_list	*next;
	size_t	pos;
	size_t	i;

	pos = 0;
	i = 0;
	if (redirections == NULL)
	{
		cmds[0]->redir_array = ft_calloc(1, sizeof(t_lex_tok *));
		return ;
	}
	ms_allocate_redirect_array(cmds, redirections);
	next = redirections;
	while (next->next != NULL)
	{
		if (((t_lex_tok *)next->content)->value != NULL)
		{
			cmds[pos]->redir_array[i] = ((t_lex_tok *)next->content);
			i++;
		}
		else
		{
			cmds[pos]->redir_array[i] = NULL;
			i = 0;
			pos += 2;
		}
		next = next->next;
	}
	if (((t_lex_tok *)next->content)->value == NULL)
	{
		cmds[pos]->redir_array[i] = NULL;
		cmds[pos + 2]->redir_array[0] = NULL;
	}
	else
	{
		cmds[pos]->redir_array[i] = next->content;
		cmds[pos]->redir_array[i + 1] = NULL;
	}
} */
