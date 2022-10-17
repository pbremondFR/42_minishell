/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 16:39:56 by pbremond          #+#    #+#             */
/*   Updated: 2022/01/13 13:52:00 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

t_tok_typ	ms_lex_get_tok_type(const char *word)
{
	size_t	len;

	len = ft_strlen(word);
	if (len == 1)
	{
		if (*word == '|')
			return (T_PIPE);
		else if (*word == ';')
			return (T_SEMI);
		else if (*word == '<')
			return (T_SLESS);
		else if (*word == '>')
			return (T_SGREAT);
	}
	else
	{
		if (ft_strnstr(word, "<<", len))
			return (T_DLESS);
		else if (ft_strnstr(word, ">>", len))
			return (T_DGREAT);
	}
	return (T_UNDEF);
}

t_lex_tok	*ms_lex_get_token(char *line, size_t *cursor)
{
	char		*word;
	t_lex_tok	*token;
	t_tok_typ	tok_type;

	if (!is_char_lex_sep(line[*cursor]))
		word = ms_lex_xtract_word_str(line, cursor, &tok_type);
	else
		word = ms_lex_xtract_word_sep(line, cursor, &tok_type);
	if (word == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_GLOBAL, false);
	token = ms_lex_create_token(word, tok_type);
	while (line[*cursor] && ft_isspace(line[*cursor]))
		*cursor += 1;
	return (token);
}

static int	lexer_error_checking(const char *line, size_t len)
{
	if (ms_lex_check_quote_parity(line, len) != 1)
	{
		ft_putstr_fd("minishell: unclosed quote\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_list	*ms_lexer(const char *line, size_t len)
{
	size_t		cursor;
	char		*line_cpy;
	t_lex_tok	*token;
	t_list		*token_node;
	t_list		*token_list;

	if (lexer_error_checking(line, len) != EXIT_SUCCESS)
		return (NULL);
	line_cpy = ft_strtrim(line, WHITESPACES);
	if (line_cpy == NULL)
		ms_exit(errno, F_ENVP | F_TOK_LST | F_GLOBAL, false);
	len = ft_strlen(line_cpy);
	cursor = 0;
	token_list = NULL;
	while (cursor < len)
	{
		token = ms_lex_get_token(line_cpy, &cursor);
		token_node = ft_lstnew(token);
		if (token_node == NULL)
			ms_exit(errno, F_ENVP | F_TOK_LST | F_GLOBAL, false);
		ft_lstadd_back(&token_list, token_node);
	}
	free(line_cpy);
	return (token_list);
}
