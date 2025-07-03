/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:58 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/03 18:39:35 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// int	main(void)
// {
// 	t_program	minishell;
// 	char		*input;
// 	t_token		*tokens;
// 	t_command	*cmd_list;

// 	minishell.envp = NULL;
// 	minishell.error_code = 0;
// 	while (1)
// 	{
// 		input = readline("minishell> ");
// 		if (!input)
// 			break ;
// 		tokens = main_lexer(input, &minishell);
// 		if (tokens)
// 		{
// 			print_tokens(tokens);
// 			cmd_list = main_parser(tokens, &minishell);
// 			if (cmd_list)
// 			{
// 				print_commands(cmd_list);
// 				free_command(cmd_list);
// 			}
// 			printf("Error code: %d\n", minishell.error_code);
// 			free_tokens(tokens);
// 		}
// 		free(input);
// 	}
// 	return (minishell.error_code);
// }

int	main(int argc, char **argv, char **envp)
{
	t_program	minishell;

	(void)argc;
	(void)argv;
	init_shell(&minishell, envp);
	run_shell(&minishell);
	free_shell(&minishell);
	return (minishell.error_code);
}
