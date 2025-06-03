/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:58 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/14 18:04:04 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int main(void)
{
	t_program	minishell;
	char		*input;
	t_token		*next;

	minishell.envp = NULL;
	minishell.error_code = 0;
	input = readline("minishell> ");
	if (!input)
		return (1);
	minishell.input_buffer = input;
	minishell.token = main_lexer(input, &minishell);
	if (minishell.token)
	{
		minishell.ast = parse_tokens(minishell.token, &minishell);
		if (minishell.ast)
			free_syntax_tree(minishell.ast);
		while (minishell.token)
		{
			next = minishell.token->next;
			free(minishell.token->value);
			free(minishell.token);
			minishell.token = next;
		}
	}
	free(input);
	return (minishell.error_code);
}
