/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:30:37 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/26 19:45:41 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handles heredoc by reading input until delimiter */
int	handle_heredoc(t_redirection *redir, const char *delim,
	t_program *minishell)
{
	int		expand;
	char	*expanded;

	expand = !(delim[0] == '\'' || delim[0] == '"');
	expanded = expand_and_remove_quotes(delim, minishell);
	if (!expanded || *expanded == '\0')
	{
		redir->target = ft_strdup(delim);
		if (!redir->target)
			return (0);
	}
	else
	{
		redir->target = ft_strdup(expanded);
		free(expanded);
		if (!redir->target)
			return (0);
	}
	redir->content = read_heredoc(redir->target, minishell, expand);
	if (!redir->content)
		return (free(redir->target), 0);
	return (1);
}

char	*expand_heredoc_line(char *line, t_program *minishell)
{
	char	*to_append;

	to_append = expand_and_remove_quotes(line, minishell);
	if (!to_append)
		minishell->error_code = ERR_MEMORY;
	return (to_append);
}

int	append_line_to_content(char **content, char *line,
	int should_free, t_program *minishell)
{
	*content = ft_strjoin_free(*content, line);
	if (!*content)
	{
		minishell->error_code = ERR_MEMORY;
		if (should_free)
			free(line);
		return (1);
	}
	*content = ft_strjoin_free(*content, "\n");
	if (!*content)
	{
		minishell->error_code = ERR_MEMORY;
		if (should_free)
			free(line);
		return (1);
	}
	return (0);
}
