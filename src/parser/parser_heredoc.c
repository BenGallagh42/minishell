/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:01:32 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 22:02:43 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint_flag = 0;

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint_flag = 1;
		printf("\n");
	}
}

static void	process_heredoc_line(char **content, char *line, int expand, t_program *minishell)
{
	char	*to_append;

	if (expand)
	{
		to_append = expand_and_remove_quotes(line, minishell);
		if (!to_append)
		{
			minishell->error_code = ERR_MEMORY; // Set error code on expansion failure
			return;
		}
	}
	else
		to_append = line;
	*content = ft_strjoin_free(*content, to_append);
	if (!*content)
	{
		minishell->error_code = ERR_MEMORY; // Set error code on join failure
		if (expand)
			free(to_append);
		return;
	}
	*content = ft_strjoin_free(*content, "\n");
	if (!*content)
	{
		minishell->error_code = ERR_MEMORY; // Set error code on newline join failure
		if (expand)
			free(to_append);
		return;
	}
	if (expand)
		free(to_append);
}

int	heredoc_input_loop(const char *delimiter, t_program *minishell, int expand, char **content)
{
	char	*line;

	while (1)
	{
		if (g_sigint_flag)
		{
			print_error_message(ERR_INTERRUPTED, NULL, minishell);
			return (1);
		}
		line = readline("> ");
		if (!line)
		{
			print_error_message(ERR_INTERRUPTED, NULL, minishell);
			return (1);
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			return (0);
		}
		process_heredoc_line(content, line, expand, minishell);
		free(line);
	}
	return (0);
}

char	*setup_and_run_heredoc(const char *delimiter, t_program *minishell, int expand)
{
	char	*content;
	void	(*old_handler)(int);

	content = ft_strdup("");
	if (!content)
		return (NULL);
	old_handler = signal(SIGINT, heredoc_signal_handler);
	if (heredoc_input_loop(delimiter, minishell, expand, &content))
	{
		free(content);
		content = NULL;
	}
	signal(SIGINT, old_handler);
	return (content);
}

char	*read_heredoc(const char *delimiter, t_program *minishell, int expand)
{
	return (setup_and_run_heredoc(delimiter, minishell, expand));
}
