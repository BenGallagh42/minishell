/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:55:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:55:04 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if a token is a word type
static int	is_word_token(t_token_type type)
{
	return (type == TKN_WORD || type == TKN_ENV || type == TKN_WILDCARD);
}

// Checks redirection tokens for errors
static int	handle_redirection(t_token **current, t_program *minishell)
{
	if (!(*current)->next || !is_word_token((*current)->next->type))
	{
		print_error_message(ERR_SYNTAX_TOKEN, (*current)->value, minishell);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}

// Processes one token and checks for syntax errors
int	process_token(t_token **current, int *has_command, t_program *minishell)
{
	if (is_word_token((*current)->type))
		*has_command = 1;
	else if (is_redirection_token((*current)->type))
	{
		if (!handle_redirection(current, minishell))
			return (0);
	}
	else if ((*current)->type == TKN_PIPE)
	{
		if (!*has_command && (*current)->next
			&& !is_redirection_token((*current)->next->type))
		{
			print_error_message(ERR_SYNTAX_PIPE, NULL, minishell);
			return (0);
		}
		*has_command = 0;
	}
	else
	{
		print_error_message(ERR_SYNTAX_TOKEN, (*current)->value, minishell);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}
