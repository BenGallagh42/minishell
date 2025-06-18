/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:50:02 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/17 19:20:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Processes word tokens into argument list */
static void	process_words(t_token **current, t_list **args,
				t_program *minishell)
{
	char	*expanded;

	while (*current && ((*current)->type == TKN_WORD
			|| (*current)->type == TKN_ENV))
	{
		expanded = expand_and_remove_quotes((*current)->value, minishell);
		if (!expanded)
		{
			ft_lstclear(args, free);
			return ;
		}
		ft_lstadd_back(args, ft_lstnew(expanded));
		*current = (*current)->next;
	}
}

/* Processes redirection tokens into redirection list */
static int	process_redirs(t_token **current, t_list **redirs,
				t_program *minishell)
{
	t_redirection	*redir;

	while (*current && is_redirection_token((*current)->type))
	{
		redir = parse_redir(current, minishell);
		if (!redir)
		{
			ft_lstclear(redirs, free_redirection);
			return (1);
		}
		ft_lstadd_back(redirs, ft_lstnew(redir));
	}
	return (0);
}

/* Processes tokens for the command, populating args and redirs lists */
static int	process_command_tokens(t_token **current, t_list **args,
								t_list **redirs, t_program *minishell)
{
	while (*current && (*current)->type != TKN_PIPE)
	{
		process_words(current, args, minishell);
		if (process_redirs(current, redirs, minishell))
			return (1);
	}
	return (0);
}

/* Finalizes the command by converting lists to arrays and performing checks */
static t_command	*finalize_command(t_command *cmd, t_list *args,
									t_list *redirs)
{
	cmd->args = list_to_array(args);
	if (!cmd->args || cmd->args[0] == NULL)
	{
		free_cmd(cmd, args, redirs);
		return (NULL);
	}
	cmd->redirs = list_to_redir_array(redirs);
	free_lists(&args, &redirs);
	return (cmd);
}

/* Parses a simple command from tokens */
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
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->is_piped = 0;
	if (process_command_tokens(&current, &args, &redirs, minishell))
	{
		free_cmd(cmd, args, redirs);
		return (NULL);
	}
	*tokens = current;
	return (finalize_command(cmd, args, redirs));
}
