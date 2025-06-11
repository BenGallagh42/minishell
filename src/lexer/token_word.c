/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:47:14 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/05 15:58:57 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Checks if a character is a special character 
static int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&'
		|| c == '\'' || c == '"');
}

// Tokenizes a word from the input, handling 
// assignment variables if '=' is present
void	token_word(const char **input, t_token **head, t_token **current)
{
	const char	*start;
	const char	*equal_position;

	start = *input;
	equal_position = NULL;
	while (**input && !ft_isspace(**input) && !is_special(**input))
	{
		if (**input == '=' && equal_position == NULL)
			equal_position = *input;
		(*input)++;
	}
	if (equal_position && equal_position > start && *input > equal_position + 1)
		add_token(head, current, TKN_ASSIGN_VAR,
			ft_strndup(start, *input - start));
	else
		add_token(head, current, TKN_WORD, ft_strndup(start, *input - start));
}
