/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrathelo <jrathelo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:05:45 by jrathelo          #+#    #+#             */
/*   Updated: 2022/01/04 13:06:13 by jrathelo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void	ms_free_token(void *token)
{
	t_lex_tok	*tok;

	tok = token;
	free(tok->value);
	tok->value = NULL;
	free(tok);
}
