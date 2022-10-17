/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/19 18:23:27 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/08 18:55:52 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Outputs error in format "minishell: (s1)[: s2]". s2 is optional.
void	ms_perror(const char *s1, const char *s2)
{
	if (s1 == NULL && s2 == NULL)
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (s1)
		ft_putstr_fd(s1, STDERR_FILENO);
	if (s2)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(s2, STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
}

// void	ms_del_token_stream(t_list **tok_list_ptr)
// {
// 	t_list		*tok_list;

// 	tok_list = *tok_list_ptr;
// 	while (tok_list)
// 	{
// 		ms_del_token(&tok_list->content);
// 		free(tok_list);
// 		tok_list = tok_list->next;
// 	}
// 	*tok_list_ptr = NULL;
// }
