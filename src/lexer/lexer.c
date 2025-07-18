/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:20:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/18 16:56:55 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_dollar_token(const char **input, t_token **head,
	t_token **current, t_program *minishell)
{
	token_dollar(input, head, current, minishell);
}

// Determines the type of token to process
// and calls the appropriate tokenization function
static void	process_lexer_token(const char **input, t_token **head,
	t_token **current, t_program *minishell)
{
	if (**input == '<' || **input == '>')
		token_redirector_main(input, head, current);
	else if (**input == '|' || **input == '&')
		token_operator(input, head, current);
	else if (**input == '$')
		handle_dollar_token(input, head, current, minishell);
	else if (has_wildcard(*input))
		token_wildcard(input, head, current);
	else
		token_word(input, head, current);
}

// Checks if the input consists only of spaces
static int	is_only_spaces(const char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}

// Main lexer function that tokenizes the input 
// and returns the head of the token list
t_token	*main_lexer(const char *input, t_program *minishell)
{
	t_token	*head;
	t_token	*current;

	head = NULL;
	current = NULL;
	if (!input || !*input || is_only_spaces(input))
		return (NULL);
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (!*input || *input == '#')
			break ;
		if (*input == '\'' || *input == '"')
		{
			token_quotes(&input, &head, &current, minishell);
			if (minishell->error_code != 0)
				break ;
		}
		else
		{
			process_lexer_token(&input, &head, &current, minishell);
		}
	}
	return (head);
}
