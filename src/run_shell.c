/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:27:05 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/29 14:27:11 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else
	{
		free(input);
		return (NULL);
	}
	return (input);
}

static void	process_input(char *input, t_program *minishell)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = main_lexer(input, minishell);
	if (!tokens)
		return ;
	commands = main_parser(tokens, minishell);
	if (commands)
	{
		execute_commands(commands, minishell);
		free_command(commands);
	}
	free_tokens(tokens);
}

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
