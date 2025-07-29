/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:30:46 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:30:48 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Checks if a character is a special token
static int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&' || c == '\''
		|| c == '"');
}

// Checks if input contains a wildcard character
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

// Tokenizes wildcard patterns
void	token_wildcard(const char **input, t_token **head, t_token **current)
{
	const char	*start;

	start = *input;
	while (**input && !ft_isspace(**input) && !is_special(**input))
		(*input)++;
	add_token(head, current, TKN_WILDCARD, ft_strndup(start, *input - start));
}
