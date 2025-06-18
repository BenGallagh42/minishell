/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:44:16 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/18 18:00:59 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_shell(t_program *minishell, char **envp)
{
	minishell->envp = envp;
	minishell->error_code = 0;
}

void	run_shell(t_program *minishell)
{
	char		*input;
	t_token		*tokens;
	t_command	*cmd_list;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		tokens = main_lexer(input, minishell);
		if (tokens)
		{
			print_tokens(tokens);
			cmd_list = main_parser(tokens, minishell);
			if (cmd_list)
			{
				print_commands(cmd_list);
				free_command(cmd_list);
			}
			printf("Error code: %d\n", minishell->error_code);
			free_tokens(tokens);
		}
		free(input);
	}
}

void	free_shell(t_program *minishell)
{
	(void)minishell;
}
