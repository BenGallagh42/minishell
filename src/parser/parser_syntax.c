/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:43:40 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/17 19:09:07 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_word_token(t_token_type type)
{
	return (type == TKN_WORD || type == TKN_ENV);
}

static void	print_syntax_error(const char *token_value, t_program *minishell)
{
	printf("minishell: syntax error near unexpected token '%s'\n", token_value);
	minishell->error_code = 2;
}

static int	validate_token_sequence(t_token *tokens, t_program *minishell)
{
	int		has_command;
	t_token	*current;

	has_command = 0;
	current = tokens;
	while (current)
	{
		if (!process_token(&current, &has_command, minishell))
			return (0);
	}
	if (!has_command)
	{
		print_syntax_error("newline", minishell);
		return (0);
	}
	return (1);
}

int	check_syntax(t_token *tokens, t_program *minishell)
{
	if (!tokens)
		return (0);
	return (validate_token_sequence(tokens, minishell));
}
