/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:58 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/05 18:44:48 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}

int	main(void)
{
	t_program	minishell;
	char		*input;
	t_token		*tokens;
	t_command	*cmd_list;

	minishell.envp = NULL;
	minishell.error_code = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		tokens = main_lexer(input, &minishell);
		if (tokens)
		{
			cmd_list = main_parser(tokens, &minishell);
			if (cmd_list)
			{
				// Execute cmd_list here (to be implemented)
				free_command(cmd_list);
			}
			free_tokens(tokens);
		}
		free(input);
	}
	return (minishell.error_code);
}
