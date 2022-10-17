/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 11:01:12 by jrathelo          #+#    #+#             */
/*   Updated: 2022/03/10 17:32:09 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

void	ms_export_list(t_list const *envp_lst)
{
	int		equal_pos;
	char	*key;
	char	*value;

	while (envp_lst)
	{
		value = NULL;
		if (ft_strchr(envp_lst->content, '=') == NULL)
			key = ft_strdup(envp_lst->content);
		else
		{
			equal_pos = ft_strchr(envp_lst->content, '=')
				- (char *)envp_lst->content;
			key = ft_substr(envp_lst->content, 0, equal_pos);
			value = ft_substr(envp_lst->content, equal_pos + 1,
					ft_strlen(envp_lst->content) - equal_pos);
		}
		if (value)
			printf("declare -x %s=\"%s\"\n", key, value);
		else
			printf("declare -x %s\n", key);
		envp_lst = envp_lst->next;
		free(key);
		free(value);
	}	
}

static int	_get_equal_pos(const char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

bool	ms_export_is_format_valid(const char *s, const char *prgm_name)
{
	int		i;
	bool	flag;

	if (s == NULL)
		return (true);
	flag = true;
	if (ft_isdigit(s[0]))
		flag = false;
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			flag = false;
		i++;
	}
	if (i == 0)
		flag = false;
	if (flag == false)
		ft_dprintf(2, "minishell: %s: `%s': not a valid identifier\n",
			prgm_name, s);
	return (flag);
}

// Attempts to modify an existing ENV, if no ENV matching its name are found
// export adds a new ENV to the back of the list.
// s must be formated in this way "ENVNAME=ENVVALUE".
int	ms_export(char *s)
{
	t_list	*env_lst;
	int		equal_pos;
	char	*new_arg;

	if (!ms_export_is_format_valid(s, "export"))
		return (EXIT_FAILURE);
	if (s == NULL)
	{
		ms_export_list(g_ms->envp);
		return (EXIT_SUCCESS);
	}
	equal_pos = _get_equal_pos(s);
	env_lst = g_ms->envp;
	new_arg = ft_strdup(s);
	while (env_lst)
	{
		if (ft_strncmp(env_lst->content, s, equal_pos) == 0)
		{
			ft_strrep((char **)&env_lst->content, new_arg);
			return (EXIT_SUCCESS);
		}
		env_lst = env_lst->next;
	}
	ft_lstadd_back(&g_ms->envp, ft_lstnew(new_arg));
	return (EXIT_SUCCESS);
}

int	ms_export_builtin(char *argv[])
{
	int	i;
	int	code;

	if (argv == NULL || argv[0] == NULL || argv[1] == NULL)
		ms_export(NULL);
	code = EXIT_SUCCESS;
	i = 0;
	while (argv[++i])
	{
		if (!ms_export_is_format_valid(argv[i], "export"))
		{
			code = EXIT_FAILURE;
			continue ;
		}
		ms_export(argv[i]);
	}
	return (code);
}
