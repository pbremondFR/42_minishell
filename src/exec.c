/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/17 16:18:18 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 18:31:25 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// Searches if current command is a builtin. Executes it and returns 0 if it is.
// Returns 1 otherwise.
int	ms_exec_builtin(t_prs_cmd *cmd)
{
	int	exit_code;

	if (ft_strncmp(cmd->token->value, "echo", 5) == 0)
		exit_code = ms_echo(cmd->argv);
	else if (ft_strncmp(cmd->token->value, "cd", 3) == 0)
		exit_code = ft_exec_cd((const char **) cmd->argv);
	else if (ft_strncmp(cmd->token->value, "pwd", 4) == 0)
		exit_code = ft_pwd();
	else if (ft_strncmp(cmd->token->value, "export", 7) == 0)
		exit_code = ms_export_builtin(cmd->argv);
	else if (ft_strncmp(cmd->token->value, "unset", 6) == 0)
		exit_code = ms_unset(cmd->argv[1]);
	else if (ft_strncmp(cmd->token->value, "env", 4) == 0)
		exit_code = env();
	else
		return (EXIT_FAILURE);
	cmd->code = exit_code;
	return (EXIT_SUCCESS);
}

int	ms_is_builtin(t_prs_cmd *cmd)
{
	if (ft_strncmp(cmd->token->value, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd->token->value, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd->token->value, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd->token->value, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd->token->value, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd->token->value, "env", 4) == 0)
		return (1);
	else
		return (0);
}

// Final step. Calls execve on pathname. If user doesn't have rights to exec,
// prints out error message based on errno.
void	ms_exec_prgm(const char *pathname, char *argv[])
{
	char	**envp;

	envp = convert_envp();
	if (access(pathname, X_OK) == 0)
		execve(pathname, argv, envp);
	else
	{
		free_array(envp);
		ms_perror(pathname, strerror(errno));
		if (errno == ENOENT)
			ms_exit(127, F_ALL, false);
		else
			ms_exit(126, F_ALL, false);
	}
}
