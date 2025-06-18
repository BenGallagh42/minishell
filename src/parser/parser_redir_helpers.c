/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:30:37 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/11 19:30:42 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Prints syntax error for redirection issues */
void	print_redir_error(t_token *current)
{
	if (!current)
		printf("minishell: syntax error near unexpected token 'newline'\n");
	else
		printf("minishell: syntax error near unexpected token '%s'\n",
			current->value);
}

/* Validates the redirection target for accessibility */
int	validate_redir_target(t_redirection *redir, t_program *minishell)
{
	if (!redir->target)
		return (0);
	if (redir->type == TKN_REDIR_IN)
	{
		if (access(redir->target, F_OK | R_OK) != 0)
		{
			printf("minishell: %s: No such file or directory\n", redir->target);
			minishell->error_code = 1;
			return (0);
		}
	}
	else if (redir->type == TKN_REDIR_OUT || redir->type == TKN_REDIR_APPEND)
	{
		if (access(redir->target, F_OK) == 0
			&& access(redir->target, W_OK) != 0)
		{
			printf("minishell: %s: Permission denied\n", redir->target);
			minishell->error_code = 1;
			return (0);
		}
	}
	return (1);
}

/* Handles heredoc by reading input until delimiter */
int	handle_heredoc(t_redirection *redir, const char *delim,
	t_program *minishell)
{
	int		expand;
	char	*content;

	expand = !(delim[0] == '\'' || delim[0] == '"');
	redir->target = remove_quotes(delim);
	if (!redir->target)
		return (0);
	content = read_heredoc(redir->target, minishell, expand);
	if (!content)
		return (free(redir->target), 0);
	redir->content = content;
	return (1);
}
