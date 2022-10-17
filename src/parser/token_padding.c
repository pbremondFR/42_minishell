/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_padding.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 17:33:28 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 16:52:42 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

static t_list	*insert_blank_node(t_list **p_tok_list, size_t i,
	t_list *cur_node)
{
	t_list	*new_node;
	t_list	*new_spot;

	new_node = ft_lstnew(ms_lex_create_token(NULL, T_BLANK));
	new_spot = ft_lstinser(*p_tok_list, i, &new_node);
	if (i == 0)
		*p_tok_list = new_spot;
	return (cur_node);
}

static void	token_list_padding(t_list **p_tok_list)
{
	t_lex_tok	*cur_tok;
	t_lex_tok	*next_tok;
	t_list		*tok_list;
	size_t		i;

	tok_list = *p_tok_list;
	cur_tok = tok_list->content;
	i = 0;
	if (ms_prs_is_sep(cur_tok->type))
		tok_list = insert_blank_node(p_tok_list, i++, tok_list);
	while (tok_list)
	{
		cur_tok = tok_list->content;
		next_tok = NULL;
		if (tok_list->next)
			next_tok = tok_list->next->content;
		if (ms_prs_is_sep(cur_tok->type)
			&& (tok_list->next == NULL || ms_prs_is_sep(next_tok->type)))
			tok_list = insert_blank_node(p_tok_list, i + 1, tok_list);
		if (tok_list)
		{
			tok_list = tok_list->next;
			i++;
		}
	}
}

void	ms_token_padding(t_list **p_tok_list, t_list **p_redir_list)
{
	if (*p_tok_list)
		token_list_padding(p_tok_list);
	if (*p_redir_list)
		token_list_padding(p_redir_list);
}
