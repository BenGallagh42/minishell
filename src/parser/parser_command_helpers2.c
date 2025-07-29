/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helpers2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:32:48 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:32:50 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks file arguments for a command
int	handle_file_validation(t_token *token,
	t_list **args, t_program *minishell)
{
	if (ft_strchr(token->value, '/')
		&& !validate_file_arg(token->value, minishell))
	{
		ft_lstclear(args, free);
		return (1);
	}
	return (0);
}

// Expands a token and adds it to arguments
int	handle_expansion_and_append(t_token *token,
	t_list **args, t_program *minishell)
{
	t_list	*expanded_list;

	expanded_list = get_expanded_list(token, minishell);
	if (!expanded_list)
	{
		ft_lstclear(args, free);
		return (1);
	}
	append_expanded(args, expanded_list);
	return (0);
}

// Creates and sets up a command structure
t_command	*init_cmd_and_set_defaults(t_program *minishell)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		print_error_message(ERR_MEMORY, NULL, minishell);
		return (NULL);
	}
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	init_command(cmd);
	return (cmd);
}

// Frees resources after a parsing error
t_command	*handle_parse_failure_cleanup(t_command *cmd,
	t_list *args, t_list *redirs)
{
	free_cmd(cmd, args, redirs);
	return (NULL);
}
