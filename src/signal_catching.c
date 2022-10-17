/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_catching.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 06:55:18 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/07 15:13:46 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void	ms_show_new_prompt(bool nl)
{
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	ms_sig_handler(int sig)
{
	if (sig == SIGINT)
		ms_show_new_prompt(true);
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ms_sig_forked_handler(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
}

void	ms_init_signals(void)
{
	signal(SIGINT, &ms_sig_handler);
	signal(SIGQUIT, &ms_sig_handler);
}
