/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:38:11 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 22:05:31 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	generate_tmp_filename(char *buffer, int count) // New function to generate unique temp file names
{
	char	*base = "/tmp/minishell_heredoc_";
	char	*num;
	int	i;

	i = 0;
	while (base[i])
	{
		buffer[i] = base[i];
		i++;
	}
	num = ft_itoa(count);
	if (!num)
		return;
	while (*num)
	{
		buffer[i] = *num++;
		i++;
	}
	buffer[i] = '\0';
}

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

static void	handle_heredoc_redir(t_redirection *redir, t_program *minishell)
{
	int		fd;
	char		tmp_file[32];
	static int	heredoc_count = 0;

	generate_tmp_filename(tmp_file, heredoc_count++); // Use custom function for unique file name
	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc");
		minishell->error_code = 1;
		return;
	}
	write(fd, redir->content, ft_strlen(redir->content));
	close(fd);
	fd = open(tmp_file, O_RDONLY);
	if (fd == -1)
	{
		perror("heredoc");
		minishell->error_code = 1;
		unlink(tmp_file);
		return;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	unlink(tmp_file);
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
			handle_heredoc_redir(redirs, minishell);
		redirs = redirs->next;
	}
}
