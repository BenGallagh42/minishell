/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:24:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/13 19:33:14 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Returns 1 if the character is a shell special character
static int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&' || c == '\''
		|| c == '"');
}

// Checks if the input string contains a wildcard (*) before any special char or space
int	has_wildcard(const char *input)
{
	while (*input && !ft_isspace(*input) && !is_special(*input))
	{
		if (*input == '*')
			return (1);
		input++;
	}
	return (0);
}

// Adds a wildcard token from input until a space or special character is found
void	token_wildcard(const char **input, t_token **head, t_token **current)
{
	const char	*start;

	start = *input;
	while (**input && !ft_isspace(**input) && !is_special(**input))
		(*input)++;
	add_token(head, current, TKN_WILDCARD, ft_strndup(start, *input - start));
}
