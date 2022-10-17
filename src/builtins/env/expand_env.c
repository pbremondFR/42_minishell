/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 14:30:47 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/10 17:26:59 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Finds the name of and ENV in a string at a specific location and returns 
// its value
char	*expand_env_get_env(const char *s, size_t i)
{
	size_t	j;
	char	*temp;
	char	*ret;

	if (ft_strncmp(s, "?", 2) == 0)
		return (ft_itoa(g_ms->last_code));
	j = i + 1;
	while (ft_isalnum(s[j]))
		j++;
	temp = ft_substr(s, i + 1, j - i + 1);
	ret = ms_find_env(temp);
	free(temp);
	return (ret);
}

// Because I forgot ft_strjoin I did some unclever reinventing, will probably
// go back and unstupid it.
size_t	expand_env_get_size(const char *s)
{
	size_t	i;
	size_t	total;
	char	*temp;

	i = 0;
	total = 0;
	while (i < ft_strlen(s))
	{
		if (s[i] == '$')
		{
			temp = expand_env_get_env(s, i);
			total += ft_strlen(temp);
			free(temp);
			i++;
			while (ft_isalnum(s[i]))
				i++;
		}
		else
		{
			total++;
			i++;
		}
	}
	return (total);
}

// Returns the value of an ENV as a malloc'ed string.
// If no ENV was found, returns NULL.
char	*ms_find_env(char *t)
{
	t_list	*envp_lst;
	char	*target;
	char	*env_var;
	size_t	target_len;

	if (ft_strncmp(t, "?", 2) == 0)
		return (ft_itoa(g_ms->last_code));
	envp_lst = g_ms->envp;
	target = ft_strjoin(t, "=");
	target_len = ft_strlen(target);
	while (envp_lst)
	{
		if (ft_strncmp(envp_lst->content, target, target_len) == 0)
		{
			env_var = envp_lst->content;
			free(target);
			return (ft_strdup(env_var + target_len));
		}
		envp_lst = envp_lst->next;
	}
	free(target);
	return (NULL);
}
