/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:57:13 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 19:30:26 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Gets user input from the prompt
static char	*get_user_input(void)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (*input)
		add_history(input);
	return (input);
}

// Processes the user’s input
static void	process_input(char *input, t_program *minishell)
{
	t_token		*tokens;
	t_command	*commands;

	if (!input[0])
		return ;
	tokens = main_lexer(input, minishell);
	if (!tokens)
		return ;
	commands = main_parser(tokens, minishell);
	if (commands)
	{
		main_executor(commands, minishell);
		free_command(commands);
	}
	free_tokens(tokens);
}

// Runs the shell in a loop
void	run_shell(t_program *minishell)
{
	char	*input;

	while (1)
	{
		input = get_user_input();
		if (!input)
			break ;
		process_input(input, minishell);
		free(input);
	}
}
