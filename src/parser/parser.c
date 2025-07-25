/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                               ，全   +#+           */
/*   Created: 2025/05/14 17:31:38 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/22 17:45:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parses a pipeline of commands separated by pipes */
t_command	*parse_pipeline(t_token **tokens, t_program *minishell)
{
	t_command	*head;
	t_command	*current;

	head = parse_simple_cmd(tokens, minishell);
	if (!head)
		return (NULL);
	current = head;
	while (*tokens && (*tokens)->type == TKN_PIPE)
	{
		*tokens = (*tokens)->next;
		current->is_piped = 1;
		current->next = parse_simple_cmd(tokens, minishell);
		if (!current->next)
		{
			free_command(head);
			return (NULL);
		}
		current = current->next;
	}
	return (head);
}

/* Performs initial checks on tokens and syntax */
static int	preliminary_checks(t_token *tokens, t_program *minishell)
{
	t_token	*current;
	int		has_non_empty;

	if (!tokens)
		return (1); // Silently return for empty input
	current = tokens;
	has_non_empty = 0;
	while (current)
	{
		if (current->value && *current->value)
			has_non_empty = 1;
		current = current->next;
	}
	if (!has_non_empty)
		return (1); // Silently return for all empty tokens
	if (has_token_errors(tokens))
	{
		print_error_message(ERR_SYNTAX_TOKEN, tokens->value, minishell);
		return (1);
	}
	if (!check_syntax(tokens, minishell))
		return (1);
	return (0);
}

/* Handles background execution for the command */
static void	handle_background(t_token **tokens, t_command *cmd)
{
	if (*tokens && (*tokens)->type == TKN_BG)
	{
		cmd->is_background = 1;
		*tokens = (*tokens)->next;
	}
}

/* Checks for remaining tokens after parsing */
static int	check_remaining_tokens(t_token *tokens, t_command **cmd,
	t_program *minishell)
{
	if (tokens)
	{
		print_error_message(ERR_SYNTAX_TOKEN, tokens->value, minishell);
		free_command(*cmd);
		*cmd = NULL;
		return (1);
	}
	return (0);
}

/* Main parser function */
t_command	*main_parser(t_token *tokens, t_program *minishell)
{
	t_command	*cmd;

	if (preliminary_checks(tokens, minishell))
		return (NULL);
	cmd = parse_pipeline(&tokens, minishell);
	if (!cmd)
		return (NULL);
	handle_background(&tokens, cmd);
	if (check_remaining_tokens(tokens, &cmd, minishell))
		return (NULL);
	return (cmd);
}
