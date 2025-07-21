/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:38:11 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 19:06:24 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirections(t_redirection *redirs, t_program *minishell)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == TKN_REDIR_OUT)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redirs->target);
				minishell->error_code = 1;
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirs->type == TKN_REDIR_APPEND)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redirs->target);
				minishell->error_code = 1;
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirs->type == TKN_REDIR_IN)
		{
			fd = open(redirs->target, O_RDONLY);
			if (fd == -1)
			{
				perror(redirs->target);
				minishell->error_code = 1;
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redirs->type == TKN_REDIR_HEREDOC)
		{
			fd = open("/tmp/minishell_heredoc", O_RDONLY);
			if (fd == -1)
			{
				perror("heredoc");
				minishell->error_code = 1;
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			unlink("/tmp/minishell_heredoc");
		}
		redirs = redirs->next;
	}
}
