/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 20:43:16 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/09 14:39:26 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>

// Takes flags to check for as a parameter, returns the binary value of all 
// flags that have been hit. Some programs function differently, for example 
// some flags overwrite others or it matters in the order that the flags are 
// pased in. This is not handeled here.
// The value returned is the binary value in order of where the flags 
// appear in the array.
// This method can only check up to 64 different flags.
size_t	ms_echo_check_flags(char **argv, int argc, const char *flags, int *i)
{
	int		j;
	int		k;
	size_t	flagger;
	size_t	temp_flagger;

	flagger = 0;
	temp_flagger = 0;
	while (*i < argc && argv[*i][0] == '-')
	{
		j = 1;
		while ((size_t) j < ft_strlen(argv[*i]))
		{
			k = 0;
			while (argv[*i][j] != flags[k])
			{
				return (flagger);
				k++;
			}
			temp_flagger |= 1 << k;
			j++;
		}
		flagger |= temp_flagger;
		(*i)++;
	}
	return (flagger);
}

int	ms_echo(char **argv)
{
	int		i;
	int		argc;
	int		nl_flag;

	nl_flag = 0;
	argc = ms_array_size((const char **)argv);
	i = 1;
	nl_flag = ms_echo_check_flags(argv, argc, "n", &i);
	while (i < argc)
	{
		if (i != argc - 1)
			printf("%s ", argv[i]);
		else
			printf("%s", argv[i]);
		++i;
	}
	if (!nl_flag)
		printf("\n");
	return (0);
}
