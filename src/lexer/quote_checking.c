/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_checking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrathelo <jrathelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:20:01 by pbremond          #+#    #+#             */
/*   Updated: 2022/01/10 14:19:38 by jrathelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Counts the number of quotes in string. When it encounters a quote, skims
// through string until matching quote is found, to avoid interpreting nested
// quotes like "Salut 'les' amis".
static void	quote_count(char const *line, int *squote, int *dquote, size_t *i)
{
	if (line[*i] == '\"')
	{
		*dquote += 1;
		*i += 1;
		while (line[*i] && line[*i] != '\"')
			*i += 1;
		if (line[*i] == '\"')
			*dquote += 1;
	}
	else if (line[*i] == '\'')
	{
		*squote += 1;
		*i += 1;
		while (line[*i] && line[*i] != '\'')
			*i += 1;
		if (line[*i] == '\'')
			*squote += 1;
	}
	*i += 1;
}

// Checks if every quote has a match. Returns 1 if quotes are okay, 0 otherwise.
// Mais peut-elle régler le problème de parité à 42 ?
int	ms_lex_check_quote_parity(char const *line, size_t len)
{
	size_t	i;
	int		squote;
	int		dquote;

	squote = 0;
	dquote = 0;
	i = 0;
	while (i < len)
		quote_count(line, &squote, &dquote, &i);
	if (squote % 2 != 0 || dquote % 2 != 0)
		return (0);
	return (1);
}
