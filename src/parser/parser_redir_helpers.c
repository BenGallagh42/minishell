/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:30:37 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/03 20:43:44 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/* Validates input redirection by checking file existence and readability */
static int	validate_input_redir(t_redirection *redir, t_program *minishell)
{
	if (!redir->target)
		return (0);
	if (access(redir->target, F_OK | R_OK) != 0)
	{
		print_error_message(ERR_FILE_NOT_FOUND, redir->target, minishell);
		return (0);
	}
	return (1);
}

/* Validates output redirection by checking write permissions */
static int	validate_output_redir(t_redirection *redir, t_program *minishell)
{
	char	*slash;
	char	*dir;

	if (!redir->target)
		return (0);
	if (access(redir->target, F_OK) == 0 && access(redir->target, W_OK) != 0)
	{
		print_error_message(ERR_PERMISSION_DENIED, redir->target, minishell);
		return (0);
	}
	if (access(redir->target, F_OK) != 0)
	{
		slash = strrchr(redir->target, '/');
		if (!slash)
			dir = ft_strdup(".");
		else
			dir = ft_substr(redir->target, 0, slash - redir->target);
		if (!dir || access(dir, W_OK) != 0)
		{
			free(dir);
			return (0);
		}
		free(dir);
	}
	return (1);
}

/* Dispatches redirection validation based on type */
int	validate_redir_target(t_redirection *redir, t_program *minishell)
{
	if (redir->type == TKN_REDIR_IN)
		return (validate_input_redir(redir, minishell));
	if (redir->type == TKN_REDIR_OUT || redir->type == TKN_REDIR_APPEND)
		return (validate_output_redir(redir, minishell));
	return (1);
}
