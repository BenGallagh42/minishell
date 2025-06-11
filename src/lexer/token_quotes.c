/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:41:50 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/05 16:01:53 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Scans the input for the closing quote character
static const char	*scan_for_quote(const char **input, char quote)
{
	while (**input && **input != quote)
		(*input)++;
	if (**input == quote)
		return (*input);
	return (NULL);
}

void	token_quotes(const char **input, t_token **head,
			t_token **current, t_program *minishell)
{
	char			quote;
	const char		*start;
	const char		*end;
	size_t			len;

	quote = **input;
	start = *input;
	(*input)++;
	end = scan_for_quote(input, quote);
	if (end)
	{
		len = end - start + 1;
		add_token(head, current, TKN_WORD, ft_strndup(start, len));
		*input = end + 1;
	}
	else
	{
		printf("minishell: syntax error near unexpected token `%c'\n", quote);
		minishell->error_code = 2;
	}
}
