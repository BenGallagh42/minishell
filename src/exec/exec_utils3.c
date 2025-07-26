/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:28:05 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/27 01:19:09 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

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

int	handle_single_builtin(t_command *cmd, t_program *mini)
{
	int	stdout_backup;

	stdout_backup = dup(STDOUT_FILENO);
	if (stdout_backup == -1)
	{
		mini->error_code = ERR_MEMORY;
		print_error_message(ERR_MEMORY, NULL, mini);
		return (0);
	}
	if (!apply_redirections(cmd->redirs, mini))
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
		return (0);
	}
	execute_builtin(cmd, mini);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdout_backup);
	return (1);
}

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
