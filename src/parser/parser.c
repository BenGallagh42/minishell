/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:44:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/30 16:53:31 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Builds a chain of commands separated by pipes
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

// Checks tokens and syntax for errors
static int	preliminary_checks(t_token *tokens, t_program *minishell)
{
	t_token	*current;
	int		has_non_empty;

	if (!tokens)
		return (1);
	current = tokens;
	has_non_empty = 0;
	while (current)
	{
		if (current->value && *current->value)
			has_non_empty = 1;
		current = current->next;
	}
	if (!has_non_empty)
		return (1);
	if (has_token_errors(tokens))
	{
		print_error_message(ERR_SYNTAX_TOKEN, tokens->value, minishell);
		return (1);
	}
	if (!check_syntax(tokens, minishell))
		return (1);
	return (0);
}

// Checks for extra tokens after parsing
static int	check_remaining_tokens(t_token *tokens, t_command **cmd,
	t_program *minishell)
{
	if (tokens && tokens->type != TKN_END)
	{
		print_error_message(ERR_SYNTAX_TOKEN, tokens->value, minishell);
		free_command(*cmd);
		*cmd = NULL;
		return (1);
	}
	return (0);
}

// Links parsed command to list and handles background processing
static void	link_command(t_cmd_list *cmd_list, t_command *cmd,
	t_token **tokens)
{
	if (!cmd_list->head)
		cmd_list->head = cmd;
	else
		cmd_list->current->next = cmd;
	cmd_list->current = cmd;
	while (cmd_list->current->next)
		cmd_list->current = cmd_list->current->next;
	handle_background(tokens, cmd_list->current);
}

// Main function to parse commands
t_command	*main_parser(t_token *tokens, t_program *minishell)
{
	t_command	*cmd;
	t_cmd_list	cmd_list;

	if (preliminary_checks(tokens, minishell))
		return (NULL);
	cmd_list.head = NULL;
	cmd_list.current = NULL;
	while (tokens)
	{
		cmd = parse_pipeline(&tokens, minishell);
		if (!cmd)
			return (free_command(cmd_list.head), NULL);
		link_command(&cmd_list, cmd, &tokens);
		if (tokens && tokens->type == TKN_END)
			tokens = tokens->next;
		if (check_remaining_tokens(tokens, &cmd, minishell))
			return (free_command(cmd_list.head), NULL);
	}
	return (cmd_list.head);
}
