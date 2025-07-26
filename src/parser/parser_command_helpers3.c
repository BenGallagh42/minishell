/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helpers3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:07:31 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/26 19:14:16 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_command	*handle_parse_failure_cleanup(t_command *cmd,
	t_list *args, t_list *redirs)
{
	free_cmd(cmd, args, redirs);
	return (NULL);
}
