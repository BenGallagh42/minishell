/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:49:40 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:49:42 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Creates a new redirection structure
static t_redirection	*allocate_redir(void)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	return (redir);
}

// Sets the redirection type and moves to the next token
static void	process_redir_type(t_token **tokens, t_redirection *redir)
{
	t_token	*current;

	current = *tokens;
	redir->type = current->type;
	*tokens = current->next;
}

// Checks for redirection syntax errors
static int	check_redir_syntax(t_token **tokens, t_program *minishell,
				t_redirection *redir)
{
	if (!*tokens || ((*tokens)->type != TKN_WORD && (*tokens)->type != TKN_ENV
			&& (*tokens)->type != TKN_WILDCARD))
	{
		if (!*tokens)
			print_error_message(ERR_SYNTAX_TOKEN, "newline", minishell);
		else
			print_error_message(ERR_SYNTAX_TOKEN, (*tokens)->value, minishell);
		free(redir);
		return (0);
	}
	if ((*tokens)->type == TKN_WILDCARD)
	{
		ft_putendl_fd("minishell: ambiguous redirect", STDERR_FILENO);
		minishell->error_code = 1;
		free(redir);
		return (0);
	}
	return (1);
}

// Expands and checks the redirection target
static int	expand_and_validate_target(t_token **tokens, t_redirection *redir,
				t_program *minishell)
{
	t_token	*current;

	current = *tokens;
	if (redir->type == TKN_REDIR_HEREDOC)
	{
		if (!handle_heredoc(redir, current->value, minishell))
			return (0);
	}
	else
	{
		redir->target = expand_and_remove_quotes(current->value, minishell);
		if (!redir->target)
			return (0);
		redir->content = NULL;
	}
	*tokens = current->next;
	return (1);
}

// Builds a redirection structure from tokens
t_redirection	*parse_redir(t_token **tokens, t_program *minishell)
{
	t_redirection	*redir;

	redir = allocate_redir();
	if (!redir)
		return (NULL);
	process_redir_type(tokens, redir);
	if (!check_redir_syntax(tokens, minishell, redir))
		return (NULL);
	if (!expand_and_validate_target(tokens, redir, minishell))
		return (NULL);
	redir->next = NULL;
	return (redir);
}
