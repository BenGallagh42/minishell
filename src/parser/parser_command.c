/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:50:02 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/26 19:13:08 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_words(t_token **current,
	t_list **args, t_program *minishell)
{
	int		is_first_arg;

	is_first_arg = 1;
	while (*current && ((*current)->type == TKN_WORD
			|| (*current)->type == TKN_WILDCARD))
	{
		if ((*current)->type == TKN_WORD
			&& !is_first_arg && needs_file_validation((*args)->content))
		{
			if (handle_file_validation(*current, args, minishell))
			{
				*current = (*current)->next;
				return (1);
			}
		}
		if (handle_expansion_and_append(*current, args, minishell))
			return (1);
		is_first_arg = 0;
		*current = (*current)->next;
	}
	return (0);
}

static int	process_redirs(t_token **current,
	t_list **redirs, t_program *minishell)
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

static t_command	*finalize_command(t_command *cmd,
	t_list *args, t_list *redirs)
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
	cmd = init_cmd_and_set_defaults(minishell);
	if (!cmd)
		return (NULL);
	if (process_command_tokens(&current, &args, &redirs, minishell))
		return (handle_parse_failure_cleanup(cmd, args, redirs));
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
