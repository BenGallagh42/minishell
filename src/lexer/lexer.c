/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:20:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/05 16:00:36 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Determines the type of token to process
// and calls the appropriate tokenization function
static void	process_token(const char **input, t_token **head, t_token **current)
{
	if (**input == '<' || **input == '>')
		token_redirector_main(input, head, current);
	else if (**input == '|' || **input == '&')
		token_operator(input, head, current);
	else if (**input == '$')
		token_dollar(input, head, current);
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
			process_token(&input, &head, &current);
		}
	}
	return (head);
}
