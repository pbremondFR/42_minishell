/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/25 02:49:54 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/09 15:34:18 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

#define PROMPT_SIZE	1024

// static t_list	*get_pwd_node_address(void)
// {
// 	t_list	*env_list;

// 	env_list = g_envp;
// 	while (env_list)
// 	{
// 		if (ft_strncmp(env_list->content, "PWD=", 4) == 0)
// 			return (env_list);
// 		env_list = env_list->next;
// 	}
// 	return (NULL);
// }

char	*get_path_with_tilde(char *home, char *path, size_t home_len)
{
	char	*tmp;
	char	*new_path;

	new_path = path;
	if (ft_strncmp(path, home, home_len) == 0
		&& ft_strlen(path) >= home_len)
	{
		if (path[home_len] == '\0' || path[home_len] == '/')
		{
			tmp = path;
			new_path = ft_strjoin("~", path + home_len);
			free(tmp);
		}
	}
	return (new_path);
}

char	*ms_prompt(void)
{
	char	*path;
	char	*prompt;
	char	*home;
	char	*line;
	size_t	home_len;

	path = ms_find_env("PWD");
	if (path == NULL)
		return (readline("minishell> "));
	home = ms_find_env("HOME");
	home_len = ft_strlen(home);
	if (home)
		path = get_path_with_tilde(home, path, home_len);
	prompt = ft_strjoin(BWHT, path);
	prompt = ft_stradd(prompt, "\e[0m> ");
	line = readline(prompt);
	free(prompt);
	free(home);
	free(path);
	return (line);
}

/* // Returns a prompt that corresponds to the current working dir. Does not use
// malloc, but rather a static array that can contain up to 1008 chars,
// including NULL terminator and excluding ANSI color codes.
char	*ms_prompt_old(void)
{
	static t_list	*pwd_node = NULL;
	static char		prompt[PROMPT_SIZE] = {0};
	char			*prompt_write;
	char			*pwd;

	if (prompt[0] == '\0' || prompt[PROMPT_SIZE - 2] == '\0')
	{
		ft_memcpy(prompt, BWHT, ft_strlen(BWHT));
		ft_memcpy(prompt + PROMPT_SIZE - (ft_strlen(RESET) + 1),
			RESET, ft_strlen(RESET));
	}
	if (pwd_node == NULL)
		pwd_node = get_pwd_node_address();
	pwd = pwd_node->content;
	if (ft_strncmp(pwd + 4, prompt,
		ft_strlen(prompt) + 1) == 0)
		return (prompt);
	prompt_write = prompt + ft_strlen(BWHT);
	ft_memccpy(prompt_write, pwd + 4, '\e',
		PROMPT_SIZE - (ft_strlen(RESET) + 1));
	return (prompt);
} */
