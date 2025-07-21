/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:30:37 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 21:51:25 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handles heredoc by reading input until delimiter */
int	handle_heredoc(t_redirection *redir, const char *delim, t_program *minishell)
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
