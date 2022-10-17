/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 11:13:40 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 17:20:40 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void	ms_exit(int code, int free_oflags, int verbose)
{
	if (verbose)
		write(STDOUT_FILENO, "exit\n", 5);
	free_global_var(free_oflags);
	exit(code);
}

static int	str_is_num(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		if (!ft_isdigit(str[i++]))
			return (0);
	return (1);
}

static int	exit_strtoimax(const char *num, intmax_t *imax_code, int sign)
{
	uintmax_t	ret_val;
	size_t		i;

	ret_val = 0;
	i = 0;
	while (ft_isdigit(num[i]))
	{
		ret_val *= 10;
		ret_val += (num[i++] - '0') % 10;
		if (ret_val > INTMAX_MAX)
		{
			if (sign > 0)
				*imax_code = INTMAX_MAX;
			else
				*imax_code = INTMAX_MIN;
			return (0);
		}
	}
	*imax_code = ret_val * sign;
	return (1);
}

static int	is_code_valid(const char *num, intmax_t *imax_code)
{
	size_t			i;
	int				sign;

	sign = 1;
	i = 0;
	while (ft_isspace(num[i]))
		i++;
	if (num[i] == '-' || num[i] == '+')
		if (num[i++] == '-')
			sign = -1;
	if (!str_is_num(&num[i]))
		return (0);
	return (exit_strtoimax(&num[i], imax_code, sign));
}

// It seems exit(3) already does the 8 bit truncating thing. At least I learned
// how it works!
void	ms_exec_exit(t_prs_cmd *cmd)
{
	int			code;
	intmax_t	imax_code;

	printf("exit\n");
	if (cmd->argv[1] == NULL)
		code = g_ms->last_code;
	else
	{
		if (!is_code_valid(cmd->argv[1], &imax_code))
		{
			ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
				cmd->argv[1]);
			code = 255;
		}
		else
			code = imax_code & 0xff;
	}
	free_global_var(F_ALL);
	exit(code);
}
