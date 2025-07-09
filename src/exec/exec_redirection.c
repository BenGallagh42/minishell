/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:38:11 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/09 20:12:17 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirections(t_redirection *redirs, t_program *minishell)
{
	t_redirection	*current;
	int				fd;

	current = redirs;
	while (current)
	{
		if (current->type == TKN_REDIR_OUT) // For >
		{
			fd = open(current->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				print_error_message(ERR_FILE_NOT_FOUND, current->target, minishell);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->type == TKN_REDIR_APPEND) // For >>
		{
			fd = open(current->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				print_error_message(ERR_FILE_NOT_FOUND, current->target, minishell);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current->type == TKN_REDIR_IN) // For <
		{
			fd = open(current->target, O_RDONLY);
			if (fd == -1)
			{
				print_error_message(ERR_FILE_NOT_FOUND, current->target, minishell);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current->type == TKN_REDIR_HEREDOC) // For <<
		{
			fd = open("/tmp/minishell_heredoc", O_RDONLY);
			if (fd == -1)
			{
				print_error_message(ERR_FILE_NOT_FOUND, "heredoc", minishell);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			unlink("/tmp/minishell_heredoc");
		}
		current = current->next;
	}
}
