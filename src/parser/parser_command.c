/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:50:02 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/03 18:03:13 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_redirection_token(t_token *token)
{
	if (token->type == TKN_REDIR_IN || token->type == TKN_REDIR_OUT)
		return (1);
	if (token->type == TKN_REDIR_APPEND || token->type == TKN_REDIR_HEREDOC)
		return (1);
	return (0);
}

static t_redirection	*parse_redir(t_token **tokens, t_program *minishell)
{
	t_redirection	*redir;
	t_token			*current;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	current = *tokens;
	redir->type = current->type;
	current = current->next;
	if (!current || current->type != TKN_WORD)
	{
		if (!current)
			printf("minishell: syntax error near unexpected token 'newline'\n");
		else
			printf("minishell: syntax error near unexpected token '%s'\n",
				current->value);
		minishell->error_code = 2;
		free(redir);
		return (NULL);
	}
	redir->target = ft_strdup(current->value);
	redir->next = NULL;
	*tokens = current->next;
	return (redir);
}

static void	process_command_tokens(t_token **current, t_list **args_list,
				t_list **redirs_list, t_program *minishell)
{
	t_redirection	*redir;

	while (*current && (*current)->type != TKN_PIPE
		&& (*current)->type != TKN_AND && (*current)->type != TKN_OR)
	{
		if ((*current)->type == TKN_WORD)
			ft_lstadd_back(args_list, ft_lstnew(ft_strdup((*current)->value)));
		else if (is_redirection_token(*current))
		{
			redir = parse_redir(current, minishell);
			if (!redir)
			{
				ft_lstclear(args_list, free);
				ft_lstclear(redirs_list, free);
				return ;
			}
			ft_lstadd_back(redirs_list, ft_lstnew(redir));
		}
		else
			break ;
		*current = (*current)->next;
	}
}

t_command	*parse_simple_cmd(t_token **tokens, t_program *minishell)
{
	t_command	*cmd;
	t_list		*args_list;
	t_list		*redirs_list;
	t_token		*current;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	args_list = NULL;
	redirs_list = NULL;
	current = *tokens;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->is_piped = 0;
	cmd->is_background = 0;
	process_command_tokens(&current, &args_list, &redirs_list, minishell);
	*tokens = current;
	cmd->args = list_to_array(args_list);
	cmd->redirs = list_to_redir_array(redirs_list);
	ft_lstclear(&args_list, free);
	ft_lstclear(&redirs_list, free);
	return (cmd);
}
