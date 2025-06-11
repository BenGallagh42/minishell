/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:01:32 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/11 16:31:07 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Processes and appends a heredoc line, with optional variable expansion */
static void	process_heredoc_line(char **content, char *line, int expand,
	t_program *minishell)
{
	char	*to_append;

	if (expand)
	{
		to_append = expand_and_remove_quotes(line, minishell);
		if (!to_append)
			return ;
	}
	else
		to_append = line;
	*content = ft_strjoin_free(*content, to_append);
	if (*content)
		*content = ft_strjoin_free(*content, "\n");
	if (expand)
		free(to_append);
}

/* Reads heredoc input until delimiter */
char	*read_heredoc(const char *delimiter, t_program *minishell, int expand)
{
	char	*content;
	char	*line;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		process_heredoc_line(&content, line, expand, minishell);
		free(line);
	}
	return (content);
}
