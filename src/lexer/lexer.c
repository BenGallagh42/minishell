/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:20:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 16:21:19 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Handles dollar sign token expansion
static void	handle_dollar_token(const char **input, t_token **head,
	t_token **current, t_program *minishell)
{
	token_dollar(input, head, current, minishell);
}

// Processes a single lexer token based on input character
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

// Checks if input contains only whitespace
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

// Handles the next token in the input, quoted or otherwise
static void	handle_input_token(const char **input, t_token **head,
	t_token **current, t_program *minishell)
{
	char	*quoted_str;

	if (**input == '\'' || **input == '"')
	{
		quoted_str = ft_strdup("");
		if (!quoted_str)
			return ;
		concatenate_quoted_string(input, &quoted_str, minishell);
		if (minishell->error_code != 0 || !quoted_str)
		{
			free(quoted_str);
			return ;
		}
		if (*quoted_str)
			add_token(head, current, TKN_WORD, quoted_str);
		else
			free(quoted_str);
	}
	else
		process_lexer_token(input, head, current, minishell);
}

// Main lexer function to tokenize input string
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
		handle_input_token(&input, &head, &current, minishell);
	}
	return (head);
}
