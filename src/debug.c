/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pbremond <pbremond@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 19:16:34 by pbremond          #+#    #+#             */
/*   Updated: 2022/03/10 18:28:16 by pbremond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

// void	debug_print_tok_list(t_list *tok_list, char *msg)
// {
// 	t_lex_tok	*token;
// 	printf("[ ↓ %s ↓\n", msg);
// 	for (int i = 0 ; tok_list ; tok_list = tok_list->next, ++i)
// 	{
// 		token = tok_list->content;
// 		printf("[ TYPE: %d\tVALUE: [%s]\n", token->type, token->value);
// 	}
// }

// void	ms_tests(t_list *tok_list, t_list *cmd_list, t_prs_cmd **cmd_array)
// {
// 	t_lex_tok	*token;
// 	t_prs_cmd	*cmd;

// 	printf("\n# -=-=-=-=-=-=-=-=-=DEBUGGING INFO=-=-=-=-=-=-=-=-=-\n");
// 	printf("#\n# <--LEXER OUTPUT-->\n");
// 	for (int i = 0 ; tok_list ; i++)
// 	{
// 		token = tok_list->content;
// 		printf("# %d: TYPE %d: >%s<\n", i, token->type, token->value);
// 		tok_list = tok_list->next;
// 	}
// 	printf("#\n# <== PARSER CMD LIST ==>\n");
// 	for (int i = 0 ; cmd_list ; i++)
// 	{
// 		cmd = cmd_list->content;
// 		token = cmd->token;
// 		printf("# cmd ID: %d / cmd type: %d\n", cmd->id, cmd->type);
// 		printf("# %d: PATH: >%s<\n", i, cmd->bin_path);
// 		if (cmd->argv)
// 		{
// 			int j = 0;
// 			for (j = 0; cmd->argv[j] ; j++) {
// 				printf("# %d: argv[%d]: >%s<\n", i, j, cmd->argv[j]);
// 			}
// 			printf("# %d: argv[%d]: >%s<\n", i, j, cmd->argv[j]);
// 		}
// 		printf("#\n");
// 		cmd_list = cmd_list->next;
// 	}
// 	if (cmd_array)
// 	{
// 		printf("#\n# <== PARSER CMD ARRAY ==>\n");
// 		for (int i = 0 ; cmd_array[i] ; i++)
// 		{
// 			cmd = cmd_array[i];
// 			token = cmd->token;
// 			printf("# cmd ID: %d / cmd type: %d\n", cmd->id, cmd->type);
// 			printf("# %d: PATH: >%s<\n", i, cmd->bin_path);
// 			if (cmd->argv)
// 			{
// 				int j = 0;
// 				for (j = 0; cmd->argv[j] ; j++) {
// 					printf("# %d: argv[%d]: >%s<\n", i, j, cmd->argv[j]);
// 				}
// 				printf("# %d: argv[%d]: >%s<\n", i, j, cmd->argv[j]);
// 			}
// 			printf("#\n");
// 		}
// 	}
// 	printf("# -=-=-=-=-=-=-=-=-=DEBUGGING INFO=-=-=-=-=-=-=-=-=-\n\n");
// }
