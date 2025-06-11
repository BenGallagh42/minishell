/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:43:40 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/11 19:59:59 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_leading_pipe(t_token *tokens, t_program *minishell)
{
	if (tokens->type == TKN_PIPE)
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		minishell->error_code = 2;
		return (0);
	}
	return (1);
}

static int	check_trailing_pipe(t_token *tokens, t_program *minishell)
{
	t_token	*last;

	last = tokens;
	while (last->next)
		last = last->next;
	if (last->type == TKN_PIPE)
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		minishell->error_code = 2;
		return (0);
	}
	return (1);
}

static int	check_redirections(t_token *tokens, t_program *minishell)
{
	while (tokens)
	{
		if (is_redirection_token(tokens->type) && !tokens->next)
		{
			printf("minishell: syntax error near unexpected token 'newline'\n");
			minishell->error_code = 2;
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *tokens, t_program *minishell)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (current->type == TKN_PIPE && current->next->type == TKN_PIPE)
		{
			printf("minishell: syntax error near unexpected token '|'\n");
			minishell->error_code = 2;
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_syntax(t_token *tokens, t_program *minishell)
{
	if (!tokens)
		return (0);
	if (!check_leading_pipe(tokens, minishell))
		return (0);
	if (!check_trailing_pipe(tokens, minishell))
		return (0);
	if (!check_redirections(tokens, minishell))
		return (0);
	if (!check_consecutive_pipes(tokens, minishell))
		return (0);
	return (1);
}
