/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/04 08:57:20 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/04 08:57:32 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static void	parse_current_redir_token(t_lex_tok *tok)
{
	if (tok->type != T_DLESS && tok->type != T_SEP)
		ft_strrep(&tok->value, ms_prs_parse_word(tok->value));
	else if (tok->type == T_DLESS)
		ft_strrep(&tok->value, ft_strtrim(tok->value, "\"'"));
}

// Fuses two tokens into one. For example:
// "TYPE: 6 - VALUE: [>]" => "TYPE: 15 - VALUE: [file]" becomes:
// "TYPE: 6 - VALUE: [file]"
// Then, parses token's value as needed. EXCEPT FOR HEREDOCS WHYYYYY BASH
t_list	*ms_preparser_2nd_pass(t_list **p_redir_list)
{
	t_list		*redir_lst;
	t_lex_tok	*tok;

	redir_lst = *p_redir_list;
	while (redir_lst)
	{
		tok = redir_lst->content;
		if ((tok->type >= T_SLESS && tok->type <= T_PIPE))
		{
			ft_strrep(&tok->value,
				ft_strdup(((t_lex_tok *)(redir_lst->next->content))->value));
			ft_lstdelone(ft_lstextr(&redir_lst, 1, 1), &ms_free_token);
		}
		redir_lst = redir_lst->next;
	}
	redir_lst = *p_redir_list;
	while (redir_lst)
	{
		tok = redir_lst->content;
		parse_current_redir_token(tok);
		redir_lst = redir_lst->next;
	}
	return (*p_redir_list);
}
