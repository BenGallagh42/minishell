/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 21:05:48 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 22:09:57 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

// Creates a string with a newline appended
static int	create_newline_string(char **with_newline, char *line,
	int should_free, t_program *minishell)
{
	if (!line)
	{
		*with_newline = NULL;
		return (0);
	}
	if (should_free)
	{
		*with_newline = ft_strjoin_free(line, "\n");
		if (!*with_newline)
		{
			minishell->error_code = ERR_MEMORY;
			return (1);
		}
	}
	else
	{
		*with_newline = ft_strjoin(line, "\n");
		if (!*with_newline)
		{
			minishell->error_code = ERR_MEMORY;
			return (1);
		}
	}
	return (0);
}

// Appends a line to the heredoc content
static int	append_line_to_content2(char **content, char *line, int should_free,
							t_program *minishell)
{
	char	*temp;
	char	*with_newline;

	if (create_newline_string(&with_newline, line, should_free, minishell))
		return (1);
	if (!with_newline)
		return (0);
	temp = ft_strjoin_free(*content, with_newline);
	if (!temp)
	{
		minishell->error_code = ERR_MEMORY;
		free(with_newline);
		return (1);
	}
	*content = temp;
	free(with_newline);
	return (0);
}

// Handles one line of heredoc input
void	process_heredoc_line(char **content, char *line, int expand,
							t_program *minishell)
{
	char	*to_append;

	if (expand)
	{
		to_append = expand_heredoc_line(line, minishell);
		if (!to_append)
		{
			minishell->error_code = ERR_MEMORY;
			return ;
		}
	}
	else
		to_append = line;
	append_line_to_content2(content, to_append, expand, minishell);
}
