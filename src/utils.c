/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:18:39 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/09 17:40:19 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

size_t	ms_array_size(const char **a)
{
	size_t	i;

	i = 0;
	while (a && a[i] != NULL)
		i++;
	return (i);
}

t_ms	*g_ms;

// Creates global variable and initializes all of its fields.
void	ms_init_global_var(const char **envp)
{
	size_t	i;

	g_ms = malloc(sizeof(t_ms));
	if (g_ms == NULL)
		exit(errno);
	if (envp[0] == NULL)
		g_ms->envp = NULL;
	else
	{
		g_ms->envp = ft_lstnew(ft_strdup(envp[0]));
		i = 1;
		while (i < ms_array_size(envp))
		{
			ft_lstadd_back(&g_ms->envp, ft_lstnew(ft_strdup(envp[i])));
			i++;
		}
	}
	g_ms->cmd_list = NULL;
	g_ms->tok_list = NULL;
	g_ms->cmd_array = NULL;
	g_ms->last_code = 0;
}

// Frees the global variable
void	free_global_var(int o_flags)
{
	if (o_flags & F_ENVP && g_ms->envp)
		ft_lstclear(&g_ms->envp, &free);
	if (o_flags & F_TOK_LST && g_ms->tok_list)
		ft_lstclear(&g_ms->tok_list, &ms_free_token);
	if (o_flags & F_CMD_LST && g_ms->cmd_list)
		ft_lstclear(&g_ms->cmd_list, &ms_free_cmd);
	if (o_flags & F_CMD_ARR && g_ms->cmd_array)
		free(g_ms->cmd_array);
	if (o_flags & F_REDIR_LST && g_ms->redir_list)
		ft_lstclear(&g_ms->redir_list, &ms_free_token);
	if (o_flags & F_GLOBAL && g_ms)
	{
		free(g_ms);
		g_ms = NULL;
	}
}

void	ms_historian(const char *line)
{
	if (ft_strlen(line) > 0)
		add_history(line);
}
