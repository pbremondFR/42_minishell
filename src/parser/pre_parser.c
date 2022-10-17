/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:35:54 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/09 21:06:32 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Pre-parser should take as an input the token stream, edit it, and output
// a new stream, containing redirection tokens.
// i.e. token list: "ls" => "-l" => ">" => "file" becomes
//					"ls" => "-l"
// And new list is					">" => "file"
// --------------------------------------------------------------------------
// token list:	"echo" => "pouet" => ">" => "file" => "pouet"
// edited to:	"echo" => "pouet"		 	=>		  "pouet"
// new list:						 ">" => "file"

// Checks if any of ( > - >> - < - << - | ) are followed by another one,
// or by nothing.
static int	ms_preprs_is_next_tok_legal(t_list *current_node)
{
	t_lex_tok	*current_tok;
	t_lex_tok	*next_tok;

	current_tok = current_node->content;
	if (current_tok->type >= T_SLESS && current_tok->type <= T_PIPE)
	{
		if (current_node->next == NULL)
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
				current_tok->value);
			return (0);
		}
		next_tok = current_node->next->content;
		if (current_tok->type != T_PIPE && next_tok->type != T_STRING)
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
				next_tok->value);
			return (0);
		}
	}
	return (1);
}

static int	ms_preparser_error_checking(t_list *tok_strm)
{
	while (tok_strm)
	{
		if (!ms_preprs_is_next_tok_legal(tok_strm))
			return (EXIT_FAILURE);
		else if (((t_lex_tok *)(tok_strm->content))->type == T_UNDEF)
		{
			ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
				((t_lex_tok *)(tok_strm->content))->value);
			return (EXIT_FAILURE);
		}
		tok_strm = tok_strm->next;
	}
	return (EXIT_SUCCESS);
}

// Returns a linked list of all the extracted tokens.
// Stops at first T_SEP encountered.
static t_list	*get_redir_until_sep(t_list **p_tok_strm, size_t *p_i)
{
	t_list		*redir_list;
	t_list		*current;
	t_lex_tok	*tok;

	redir_list = NULL;
	current = ft_lstgetn(*p_tok_strm, *p_i);
	tok = current->content;
	while (current && !(tok->type == T_PIPE || tok->type == T_SEMI))
	{
		if (is_sless_dless(tok->type) || is_sgreat_dgreat(tok->type))
		{
			ft_lstadd_back(&redir_list, ft_lstextr(p_tok_strm, *p_i, 2));
			current = ft_lstgetn(*p_tok_strm, *p_i);
		}
		else
		{
			current = current->next;
			*p_i += 1;
		}
		if (current)
			tok = current->content;
	}
	return (redir_list);
}

// The preparser's job is to isolate all redirection tokens to a separate linked
// list. These will then be mapped to an array and treated separately from the
// command's arguments.
t_list	*ms_preparser(t_list **p_tok_strm)
{
	t_list		*redir_list;
	size_t		i;

	if (ms_preparser_error_checking(*p_tok_strm) != EXIT_SUCCESS)
	{
		ft_lstclear(&g_ms->tok_list, &ms_free_token);
		return (NULL);
	}
	i = 0;
	redir_list = NULL;
	while (ft_lstgetn(*p_tok_strm, i))
	{
		ft_lstadd_back(&redir_list, get_redir_until_sep(p_tok_strm, &i));
		if (ft_lstgetn(*p_tok_strm, i++))
			ft_lstadd_back(&redir_list,
				ft_lstnew(ms_lex_create_token(NULL, T_SEP)));
	}
	return (ms_preparser_2nd_pass(&redir_list));
}
