/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 19:32:46 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/04 08:06:59 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Extracts the next "word" in line. Increments *cursor value, and defines
// *tok_type, so they can later be used in calling function.
char	*ms_lex_xtract_word_str(char *line, size_t *cursor, t_tok_typ *tok_type)
{
	size_t	start;
	char	*word;

	start = *cursor;
	while (line[*cursor] && !is_char_lex_sep(line[*cursor]))
	{
		if (line[*cursor] == '"')
		{
			*cursor += 1;
			while (line[*cursor] && line[*cursor] != '"')
				*cursor += 1;
		}
		else if (line[*cursor] == '\'')
		{
			*cursor += 1;
			while (line[*cursor] && line[*cursor] != '\'')
				*cursor += 1;
		}
		*cursor += 1;
	}
	word = ft_substr(line, (unsigned int)start, *cursor - start);
	*tok_type = T_STRING;
	return (word);
}

static size_t	get_max_token_len(const char c)
{
	if (c == '|' || c == ';')
		return (1);
	else if (c == '<' || c == '>')
		return (2);
	else
		return (0);
}

// Extracts the next "word", which is to be an operator/separator. Otherwise
// works the same way as ms_lex_xtract_word_str();
char	*ms_lex_xtract_word_sep(char *line, size_t *cursor, t_tok_typ *tok_type)
{
	size_t	start;
	size_t	max_tok_len;
	char	*word;

	start = *cursor;
	max_tok_len = get_max_token_len(line[*cursor]);
	while (line[*cursor] && is_char_lex_sep(line[*cursor])
		&& !ft_isspace(line[*cursor]) && *cursor - start < max_tok_len)
	{
		if (line[*cursor] != line[start])
			break ;
		*cursor += 1;
	}
	word = ft_substr(line, (unsigned int)start, *cursor - start);
	*tok_type = ms_lex_get_tok_type(word);
	return (word);
}
