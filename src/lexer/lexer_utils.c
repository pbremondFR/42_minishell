/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:33:18 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/07 19:16:58 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Returns a token corresponding to given character.
t_tok_typ	lex_char_check(char c)
{
	if (c == '"')
		return (T_DQUOTE);
	else if (c == '\'')
		return (T_SQUOTE);
	else if (c == '<')
		return (T_SLESS);
	else if (c == '>')
		return (T_SGREAT);
	else if (c == '|')
		return (T_PIPE);
	else if (c == ';')
		return (T_SEMI);
	else if (ft_isalnum(c))
		return (T_ALNUM);
	else
		return (T_UNDEF);
}

// Returns 1 if given char is a separator for the lexer. Whitespaces, pipes,
// semicolons, redirections, etc.
int	is_char_lex_sep(char c)
{
	t_tok_typ	tok_type;

	tok_type = lex_char_check(c);
	if (ft_isspace(c) || tok_type == T_SEMI || tok_type == T_SLESS
		|| tok_type == T_SGREAT || tok_type == T_PIPE)
		return (1);
	return (0);
}

// Returns 1 if given token type is a separator
int	is_tok_lex_sep(t_tok_typ type)
{
	if (type == T_SEMI || type == T_SLESS || type == T_SGREAT
		|| type == T_DLESS || type == T_DGREAT || type == T_PIPE
		|| type == T_SEP || type == T_BLANK)
		return (1);
	return (0);
}

// mallocs a new token. char *word is already malloc'ed elsewhere.
// Both the token's char* fields and the token itself are be malloc'ed,
// so don't use a stack string or you will crash later on.
// These are malloc'ed so we can assign them to a chained list.
t_lex_tok	*ms_lex_create_token(char *word, t_tok_typ type)
{
	t_lex_tok	*token;

	token = malloc(sizeof(t_lex_tok));
	if (token == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_GLOBAL, false);
	token->type = type;
	token->value = word;
	return (token);
}

// Frees and NULLs and token fields, then does it to token itself.
void	ms_del_token(t_lex_tok **token_ptr)
{
	char		*tmp;
	t_lex_tok	*token;

	token = *token_ptr;
	tmp = token->value;
	token->value = NULL;
	free(tmp);
	*token_ptr = NULL;
	free(token);
}
