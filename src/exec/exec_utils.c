/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 02:04:44 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/29 19:31:13 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Sets up signal handlers for child process
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Configures pipes for command execution
void	setup_pipes(int prev_pipe, int *pipefd, int is_piped)
{
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (is_piped)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

// Applies redirections to the current command
int	apply_redirections(t_redirection *redirs, t_program *mini)
{
	int				fd;
	static int		heredoc_count;
	t_heredoc_ctx	ctx;

	ctx.fd = &fd;
	ctx.heredoc_count = &heredoc_count;
	return (process_redirection_list(redirs, mini, &ctx));
}

// Sets up pipe and forks for command execution
int	setup_pipe_and_fork(t_command *cmd, t_program *mini,
	int *pipefd, pid_t *pid)
{
	if (cmd->is_piped && pipe(pipefd) == -1)
	{
		mini->error_code = ERR_PIPE;
		print_error_message(ERR_PIPE, NULL, mini);
		return (0);
	}
	*pid = fork();
	if (*pid == -1)
	{
		mini->error_code = ERR_FORK;
		print_error_message(ERR_FORK, NULL, mini);
		return (0);
	}
	return (1);
}

// Executes child process with redirections and pipes
void	exec_child(t_command *cmd, t_program *mini,
	int prev_pipe, int *pipefd)
{
	setup_child_signals();
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	setup_pipes(prev_pipe, pipefd, cmd->is_piped);
	if (!apply_redirections(cmd->redirs, mini))
	{
		free_command(cmd);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
		handle_missing_args_and_exit(cmd, mini);
	validate_and_exec_command(cmd, mini);
}
