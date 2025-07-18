/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 20:44:34 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/18 17:38:23 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Allocates memory for a new redirection structure */
static t_redirection	*allocate_redir(void)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	return (redir);
}

/* Sets the redirection type and advances the token pointer */
static void	process_redir_type(t_token **tokens, t_redirection *redir)
{
	t_token	*current;

	current = *tokens;
	redir->type = current->type;
	*tokens = current->next;
}

/* Checks for syntax errors in redirection (e.g., missing target) */
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

/* Expands and validates the redirection target, handles heredoc if needed */
static int	expand_and_validate_target(t_token **tokens, t_redirection *redir,
				t_program *minishell)
{
	t_token	*current;

	current = *tokens;
	if (redir->type == TKN_REDIR_HEREDOC)
	{
		if (!handle_heredoc(redir, current->value, minishell))
		{
			free(redir);
			return (0);
		}
	}
	else
	{
		redir->target = expand_and_remove_quotes(current->value, minishell);
		if (!redir->target)
		{
			free(redir->target);
			free(redir);
			return (0);
		}
		redir->content = NULL;
	}
	*tokens = current->next;
	return (1);
}

/* Parses a redirection token and its target,
returning a redirection structure */
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
