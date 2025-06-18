/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:31:38 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/18 17:48:36 by bboulmie         ###   ########.fr       */
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
			return (free_command(head), NULL);
		current = current->next;
	}
	return (head);
}

/* Initializes the parsing process */
t_command	*main_parser(t_token *tokens, t_program *minishell)
{
	t_command	*cmd;

	if (!tokens || !tokens->value || !*tokens->value)
	{
		minishell->error_code = 0;
		return (NULL);
	}
	if (!check_syntax(tokens, minishell))
		return (NULL);
	cmd = parse_pipeline(&tokens, minishell);
	if (!cmd)
		return (NULL);
	if (tokens && tokens->type == TKN_BG)
	{
		cmd->is_background = 1;
		tokens = tokens->next;
	}
	if (tokens)
	{
		printf("minishell: syntax error");
		minishell->error_code = 2;
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
