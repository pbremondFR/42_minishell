/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 19:22:00 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/11 18:05:10 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

static char	*expand_normal_string(char *original)
{
	char	*new_str;
	char	*tmp;

	new_str = NULL;
	if (ft_strchr(original, '~'))
		new_str = ms_prs_exp_tilde(original, ft_strlen(original));
	if (ft_strchr(original, '$'))
	{
		if (new_str)
			tmp = new_str;
		else
			tmp = ft_strdup(original);
		new_str = ms_prs_exp_str_env(tmp, ft_strlen(tmp));
		free(tmp);
	}
	return (new_str);
}

static char	*unquoted_str_parsing(char *word, size_t *cursor)
{
	size_t	i;
	char	*original;
	char	*new_str;

	i = 0;
	while (word[*cursor + i] && word[*cursor + i] != '\''
		&& word[*cursor + i] != '\"')
		++i;
	original = ft_substr(word, *cursor, i);
	if (i == 0)
		i++;
	*cursor += i - 1;
	if (!ft_strchr(original, '~') && !ft_strchr(original, '$'))
		return (original);
	new_str = expand_normal_string(original);
	free(original);
	return (new_str);
}

static char	*dquote_str_parsing(char *word, size_t *cursor)
{
	size_t	i;
	char	*tmp;
	char	*new_str;

	*cursor += 1;
	i = 0;
	while (word[*cursor + i] && word[*cursor + i] != '\"')
		++i;
	tmp = ft_substr(word, *cursor, i);
	if (tmp == NULL)
		return (NULL);
	*cursor += i;
	new_str = ms_prs_exp_str_env(tmp, ft_strlen(tmp));
	free(tmp);
	return (new_str);
}

static char	*squote_str_parsing(char *word, size_t *cursor)
{
	size_t	i;
	char	*new_str;

	*cursor += 1;
	i = 0;
	while (word[*cursor + i] && word[*cursor + i] != '\'')
		++i;
	new_str = ft_substr(word, *cursor, i);
	*cursor += i;
	return (new_str);
}

char	*ms_prs_parse_word(char *word)
{
	size_t	i;
	char	*new_word;
	char	*tmp;

	new_word = ft_strnew(0);
	i = 0;
	while (word[i])
	{
		if (word[i] != '\"' && word[i] != '\'')
			tmp = unquoted_str_parsing(word, &i);
		else if (word[i] == '\"')
			tmp = dquote_str_parsing(word, &i);
		else if (word[i] == '\'')
			tmp = squote_str_parsing(word, &i);
		new_word = ft_stradd(new_word, tmp);
		free(tmp);
		if (word[i])
			++i;
	}
	return (new_word);
}
