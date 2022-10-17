/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expanding.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 07:22:37 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/04 11:48:57 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

char	*ms_prs_exp_tilde(const char *word, size_t word_len)
{
	char	*new_str;
	char	*home;
	size_t	i;

	(void)word_len;
	home = ms_find_env("HOME");
	if (home == NULL)
		return (ft_strdup(word));
	i = 0;
	while (word[i] && word[i] != '~')
		i++;
	new_str = ft_substr(word, 0, i++);
	new_str = ft_stradd(new_str, home);
	new_str = ft_stradd(new_str, &word[i]);
	free(home);
	return (new_str);
}
