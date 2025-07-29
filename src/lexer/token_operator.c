/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:31:08 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:31:10 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Checks for consecutive operators and sets an error if found
void	check_consecutive_operators(const char *input, t_token *current_token)
{
	const char	*next_char;

	next_char = input;
	while (*next_char && ft_isspace(*next_char))
	{
		next_char++;
	}
	if (*next_char == '<' || *next_char == '>' || *next_char == '|'
		|| *next_char == '&')
	{
		ft_printf("minishell: syntax error");
		current_token->error = 1;
	}
}

// Determines the type of operator token
t_token_type	get_operator_type(const char **input)
{
	if (**input == '|' && *(*input + 1) == '|')
	{
		*input += 2;
		return (TKN_OR);
	}
	else if (**input == '|')
	{
		(*input)++;
		return (TKN_PIPE);
	}
	else if (**input == '&' && *(*input + 1) == '&')
	{
		*input += 2;
		return (TKN_AND);
	}
	else
	{
		(*input)++;
		return (TKN_BG);
	}
}

// Tokenizes operators and checks for consecutive operator errors
void	token_operator(const char **input, t_token **head,
	t_token **current)
{
	const char		*start;
	t_token_type	type;

	start = *input;
	type = get_operator_type(input);
	add_token(head, current, type, ft_strndup(start, *input - start));
	check_consecutive_operators(*input, *current);
}
