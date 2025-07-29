/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:33:33 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/29 16:42:56 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Generates a temporary filename for heredoc
void	generate_tmp_filename(char *buffer, int count)
{
	char	*base;
	char	*num;
	int		i;

	base = "/tmp/minishell_heredoc_";
	i = 0;
	while (base[i])
	{
		buffer[i] = base[i];
		i++;
	}
	num = ft_itoa(count);
	if (!num)
		return ;
	while (*num)
	{
		buffer[i] = *num++;
		i++;
	}
	buffer[i] = '\0';
}

// Handles output redirection
int	handle_output_redirection(t_redirection *redir, t_program *mini, int *fd)
{
	int	err_code;

	if (redir->type == TKN_REDIR_OUT)
		*fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		*fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd == -1)
	{
		if (errno == EACCES || errno == EISDIR)
			err_code = ERR_PERMISSION_DENIED;
		else
			err_code = ERR_FILE_NOT_FOUND;
		return (print_error_message(err_code, redir->target, mini), 0);
	}
	dup2(*fd, STDOUT_FILENO);
	close(*fd);
	return (1);
}

// Handles input redirection
int	handle_input_redirection(t_redirection *redir, t_program *mini, int *fd)
{
	int	err_code;

	if (!redir->target || !*redir->target)
		return (print_error_message(ERR_FILE_NOT_FOUND, NULL, mini), 0);
	*fd = open(redir->target, O_RDONLY);
	if (*fd == -1)
	{
		if (errno == EACCES)
			err_code = ERR_PERMISSION_DENIED;
		else
			err_code = ERR_FILE_NOT_FOUND;
		return (print_error_message(err_code, redir->target, mini), 0);
	}
	dup2(*fd, STDIN_FILENO);
	close(*fd);
	return (1);
}

// Handles heredoc redirection
int	handle_heredoc_redirection(t_redirection *redir, t_program *mini,
	t_heredoc_ctx *ctx)
{
	generate_tmp_filename(ctx->tmp_file, (*ctx->heredoc_count)++);
	*(ctx->fd) = open(ctx->tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*(ctx->fd) == -1)
		return (print_error_message(ERR_FILE_NOT_FOUND, "heredoc", mini), 0);
	write(*(ctx->fd), redir->content, ft_strlen(redir->content));
	close(*(ctx->fd));
	*(ctx->fd) = open(ctx->tmp_file, O_RDONLY);
	if (*(ctx->fd) == -1)
	{
		unlink(ctx->tmp_file);
		return (print_error_message(ERR_FILE_NOT_FOUND, "heredoc", mini), 0);
	}
	dup2(*(ctx->fd), STDIN_FILENO);
	close(*(ctx->fd));
	unlink(ctx->tmp_file);
	return (1);
}

// Processes a list of redirections
int	process_redirection_list(t_redirection *redirs,
	t_program *mini, t_heredoc_ctx *ctx)
{
	while (redirs)
	{
		if (redirs->type == TKN_REDIR_OUT || redirs->type == TKN_REDIR_APPEND)
		{
			if (!handle_output_redirection(redirs, mini, ctx->fd))
				return (0);
		}
		else if (redirs->type == TKN_REDIR_IN)
		{
			if (!handle_input_redirection(redirs, mini, ctx->fd))
				return (0);
		}
		else if (redirs->type == TKN_REDIR_HEREDOC)
		{
			if (!handle_heredoc_redirection(redirs, mini, ctx))
				return (0);
		}
		redirs = redirs->next;
	}
	return (1);
}
