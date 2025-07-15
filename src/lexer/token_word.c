/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:47:14 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/14 21:28:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Checks if a character is a special character
static int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&'
		|| c == '\'' || c == '"');
}

// Handles the value part after '=' in a word token
static void	handle_value_part(const char **input, char quote)
{
	if (**input == '"' || **input == '\'')
	{
		quote = **input;
		(*input)++;
		while (**input && **input != quote)
			(*input)++;
		if (**input == quote)
			(*input)++;
	}
	else
	{
		while (**input && !ft_isspace(**input) && !is_special(**input))
			(*input)++;
	}
}

// Tokenizes a word from the input
void	token_word(const char **input, t_token **head, t_token **current)
{
	const char	*start;
	char		quote;

	start = *input;
	quote = '\0';
	while (**input && !ft_isspace(**input) && !is_special(**input))
	{
		if (**input == '=')
		{
			(*input)++;
			handle_value_part(input, quote);
			break ;
		}
		(*input)++;
	}
	add_token(head, current, TKN_WORD, ft_strndup(start, *input - start));
}
