/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expanding.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 23:39:15 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 16:12:29 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// I think I got carried away here. Anyway, it's hopefully done, and I hopefully
// won't have to touch this code ever again. please.
// On the bright side, it should be more efficient, since it only mallocs once
// for the new string.

// Why yes, this is ALSO to get around the norm. If you know how to do it
// better, feel free to edit it. I'm too tired.
static void	mv_cursor_to_end_of_envar(const char *word, size_t *cursor)
{
	size_t	i;

	i = 0;
	if (word[*cursor + 1] == '{' && ft_strchr(&word[*cursor + 1], '}'))
		while (word[*cursor + i] && word[*cursor + i] != '}')
			i++;
	else
		while (word[*cursor + i] && ms_prs_in_envar_name(&word[*cursor], i))
			i++;
	*cursor += i;
}

// Get the future length of given string once all env vars are expanded.
size_t	ms_prs_get_exp_strlen(const char *word)
{
	size_t	len;
	size_t	i;
	char	*envar_name;
	char	*envar_val;

	len = 0;
	i = 0;
	while (word[i])
	{
		if (word[i] == '$')
		{
			envar_name = ms_prs_get_envar_name(&word[i + 1]);
			envar_val = ms_find_env(envar_name);
			len += ft_strlen(envar_val);
			free(envar_name);
			free(envar_val);
			mv_cursor_to_end_of_envar(word, &i);
		}
		if (word[i])
		{
			++i;
			++len;
		}
	}
	return (len);
}

// Why yes, this is just to get around the norm, how did you know ?
static char	*get_curlbracket_envar_name(const char *word, size_t *word_cursor,
	size_t *i)
{
	char	*envar_name;

	*i += 1;
	*word_cursor += 1;
	while (word[*word_cursor + *i] && word[*word_cursor + *i] != '}')
		*i += 1;
	*word_cursor += *i;
	envar_name = ft_substr(word, *word_cursor - *i, *i);
	*word_cursor += 1;
	return (envar_name);
}

// Copies into new_str the value of the environment var pointed to by *word.
// To be used with parent function, which goes through given parser's "word"
// and expands all env vars found into new_str.
static void	expand_envar_into_string(const char *word, char *new_str,
	size_t *w_cursor, size_t *n_cursor)
{
	char	*envar_val;
	char	*envar_name;
	size_t	i;

	*w_cursor += 1;
	i = 0;
	if (word[*w_cursor] == '{' && ft_strchr(word + *w_cursor, '}'))
		envar_name = get_curlbracket_envar_name(word, w_cursor, &i);
	else
	{
		while (word[*w_cursor + i] && ms_prs_in_envar_name(&word[*w_cursor], i))
			++i;
		*w_cursor += i;
		envar_name = ft_substr(word, *w_cursor - i, i);
	}
	envar_val = ms_find_env(envar_name);
	if (envar_name && envar_val)
		ft_memcpy(new_str + *n_cursor, envar_val, ft_strlen(envar_val));
	*n_cursor += ft_strlen(envar_val);
	free(envar_name);
	free(envar_val);
}

// Returns a malloc'ed string with all environment variables expanded. Returns
// a copy of the input string if no env vars are found.
// Returns NULL if failed.
char	*ms_prs_exp_str_env(const char *word, size_t word_len)
{
	char	*new_str;
	size_t	new_len;
	size_t	word_i;
	size_t	new_i;

	new_len = ms_prs_get_exp_strlen(word);
	new_str = ft_calloc(new_len + 1, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	word_i = 0;
	new_i = 0;
	while (word_i < word_len)
	{
		if (word[word_i] == '$')
			expand_envar_into_string(word, new_str, &word_i, &new_i);
		if (word[word_i] == '$')
			continue ;
		new_str[new_i] = word[word_i];
		++word_i;
		++new_i;
	}
	return (new_str);
}
