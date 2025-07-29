/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:50:42 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:50:44 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks the order of tokens for errors
static int	validate_token_sequence(t_token *tokens, t_program *minishell)
{
	int		has_command;
	t_token	*current;

	has_command = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TKN_BG && current->next)
		{
			print_syntax_error(current->value, minishell);
			return (0);
		}
		if (!process_token(&current, &has_command, minishell))
			return (0);
	}
	if (!has_command)
	{
		print_error_message(ERR_SYNTAX_TOKEN, "newline", minishell);
		return (0);
	}
	return (1);
}

// Checks if tokens follow correct syntax
int	check_syntax(t_token *tokens, t_program *minishell)
{
	if (!tokens)
		return (0);
	return (validate_token_sequence(tokens, minishell));
}
