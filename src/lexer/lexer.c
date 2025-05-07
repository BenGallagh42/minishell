/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:20:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/07 18:26:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*main_lexer(const char *input, t_program *minishell)
{
	t_token	*head;
	t_token	*current;

	head = NULL;
	current = NULL;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (!*input || *input == '#')
			break ;
		if (*input == '<' || *input == '>')
			token_redirector_main(&input, &head, &current);
		else if (*input == '|' || *input == '&')
			token_operator(&input, &head, &current);
		else if (*input == '$')
			token_dollar(&input, &head, &current);
		else if (*input == '\'' || *input == '"')
			token_quotes(&input, &head, &current, minishell);
		else if (*input == '*')
			token_wildcard(&input, &head, &current);
		else
			token_word(&input, &head, &current);
	}
	return (head);
}
