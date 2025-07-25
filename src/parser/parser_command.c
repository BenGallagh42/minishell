/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:50:02 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/24 20:04:03 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_words(t_token **current, t_list **args, t_program *minishell)
{
	t_list	*expanded_list;
	int		is_first_arg;

	is_first_arg = 1;
	while (*current && ((*current)->type == TKN_WORD
			|| (*current)->type == TKN_WILDCARD))
	{
		if ((*current)->type == TKN_WORD
			&& !is_first_arg && needs_file_validation((*args)->content))
		{
			if (ft_strchr((*current)->value, '/')
				&& !validate_file_arg((*current)->value, minishell))
			{
				ft_lstclear(args, free);
				*current = (*current)->next;
				return (1);
			}
		}
		expanded_list = get_expanded_list(*current, minishell);
		if (!expanded_list)
		{
			ft_lstclear(args, free);
			return (1);
		}
		append_expanded(args, expanded_list);
		is_first_arg = 0;
		*current = (*current)->next;
	}
	return (0);
}

static int	process_redirs(t_token **current, t_list **redirs, t_program *minishell)
{
	t_redirection	*redir;
	t_list			*new_node;

	while (*current && is_redirection_token((*current)->type))
	{
		redir = parse_redir(current, minishell);
		if (!redir)
		{
			ft_lstclear(redirs, free_redirection);
			return (1);
		}
		new_node = ft_lstnew(redir);
		if (!new_node)
		{
			free_redirection(redir);
			ft_lstclear(redirs, free_redirection);
			return (1);
		}
		ft_lstadd_back(redirs, new_node);
	}
	return (0);
}

static int	process_command_tokens(t_token **current, t_list **args,
	t_list **redirs, t_program *minishell)
{
	while (*current && (*current)->type != TKN_PIPE)
	{
		if (is_redirection_token((*current)->type))
		{
			if (process_redirs(current, redirs, minishell))
			{
				ft_lstclear(redirs, free_redirection);
				return (1);
			}
		}
		else
		{
			if (process_words(current, args, minishell))
			{
				ft_lstclear(redirs, free_redirection);
				return (1);
			}
		}
	}
	return (0);
}

static t_command	*finalize_command(t_command *cmd, t_list *args, t_list *redirs)
{
	cmd->args = list_to_array(args);
	if (!cmd->args || cmd->args[0] == NULL)
	{
		free_cmd(cmd, args, redirs);
		return (NULL);
	}
	cmd->redirs = list_to_redir_array(redirs);
	if (!cmd->redirs && redirs)
	{
		ft_lstclear(&redirs, free_redirection);
		free_cmd(cmd, args, NULL);
		return (NULL);
	}
	if (args)
		ft_lstclear(&args, free);
	if (redirs)
		ft_lstclear(&redirs, free_redirection);
	return (cmd);
}

t_command	*parse_simple_cmd(t_token **tokens, t_program *minishell)
{
	t_command	*cmd;
	t_list		*args;
	t_list		*redirs;
	t_token		*current;

	args = NULL;
	redirs = NULL;
	current = *tokens;
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
	if (process_command_tokens(&current, &args, &redirs, minishell))
	{
		free_cmd(cmd, args, redirs);
		return (NULL);
	}
	*tokens = current;
	cmd = finalize_command(cmd, args, redirs);
	if (!cmd)
	{
		free(cmd);
		return (NULL);
	}
	if (cmd->args && cmd->args[0])
		cmd->is_builtin = is_builtin(cmd->args[0]);
	return (cmd);
}
