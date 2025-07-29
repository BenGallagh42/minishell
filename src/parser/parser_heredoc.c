/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:29:39 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 22:01:15 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint_flag = 0;

// Checks heredoc input for interrupts, EOF, or delimiter
static int	handle_heredoc_input(const char *delimiter, char *line,
							t_program *minishell)
{
	if (g_sigint_flag)
	{
		print_error_message(ERR_INTERRUPTED, NULL, minishell);
		return (1);
	}
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document "
			"delimited by end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd((char *)delimiter, STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
		return (0);
	}
	if (ft_strcmp(line, delimiter) == 0)
		return (0);
	return (-1);
}

// Reads heredoc input until the delimiter
int	heredoc_input_loop(const char *delimiter, t_program *minishell,
						int expand, char **content)
{
	char	*line;
	int		result;

	while (1)
	{
		line = readline("> ");
		result = handle_heredoc_input(delimiter, line, minishell);
		if (result >= 0)
		{
			free(line);
			return (result);
		}
		process_heredoc_line(content, line, expand, minishell);
		free(line);
	}
	return (0);
}

// Handles Ctrl+C during heredoc input
static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint_flag = 1;
		printf("\n");
	}
}

// Sets up and collects heredoc input
char	*setup_and_run_heredoc(const char *delimiter,
							t_program *minishell, int expand)
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
		return (NULL);
	}
	if (minishell->error_code == ERR_MEMORY)
	{
		free(content);
		return (NULL);
	}
	signal(SIGINT, old_handler);
	return (content);
}

// Starts heredoc processing for a delimiter
char	*read_heredoc(const char *delimiter, t_program *minishell, int expand)
{
	return (setup_and_run_heredoc(delimiter, minishell, expand));
}
