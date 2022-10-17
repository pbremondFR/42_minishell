/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expanding_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 05:15:15 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 17:26:35 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

bool	ms_prs_in_envar_name(const char *s, size_t i)
{
	if (!(ft_isalnum(s[i]) || s[i] == '_') && !(i == 0 && s[i] == '?'))
		return (false);
	if (i == 1 && ft_isdigit(s[i - 1]))
		return (false);
	return (true);
}

// Returns a malloc'ed string containing the isolated name of envar
// that str points to, to be used with ms_find_env().
// Accepts $FOO and ${FOO} formats.
char	*ms_prs_get_envar_name(const char *str)
{
	char	*envar_name;
	size_t	i;

	i = 0;
	if (str[i] == '{' && ft_strchr(&str[i], '}'))
	{
		++i;
		while (str[i] && str[i] != '}')
			++i;
		envar_name = ft_substr(str, 1, i - 1);
	}
	else
	{
		while (str[i] && ms_prs_in_envar_name(str, i))
			++i;
		envar_name = ft_substr(str, 0, i);
	}
	return (envar_name);
}
