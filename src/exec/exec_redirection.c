/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:38:11 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/15 12:20:29 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_output_redir(t_redirection *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->target);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_append_redir(t_redirection *redir)
{
	int	fd;

	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->target);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_input_redir(t_redirection *redir)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->target);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	handle_heredoc_redir(void)
{
	int	fd;

	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	if (fd == -1)
	{
		perror("heredoc");
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink("/tmp/minishell_heredoc");
}

void	handle_redirections(t_redirection *redirs, t_program *minishell)
{
	while (redirs)
	{
		if (redirs->type == TKN_REDIR_OUT)
			handle_output_redir(redirs);
		else if (redirs->type == TKN_REDIR_APPEND)
			handle_append_redir(redirs);
		else if (redirs->type == TKN_REDIR_IN)
			handle_input_redir(redirs);
		else if (redirs->type == TKN_REDIR_HEREDOC)
			handle_heredoc_redir();
		redirs = redirs->next;
	}
	(void)minishell;
}
