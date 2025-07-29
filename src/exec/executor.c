/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 19:30:49 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Executes a single built-in command with redirections
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

// Main loop for executing commands
void	exec_loop(t_command *cmd, t_program *mini, pid_t *pids, int count)
{
	int	i;
	int	prev_pipe;
	int	pipefd[2];

	i = -1;
	prev_pipe = -1;
	while (++i < count)
	{
		if (cmd->is_builtin && count == 1)
		{
			if (!handle_single_builtin(cmd, mini))
				return ;
			return ;
		}
		if (!setup_pipe_and_fork(cmd, mini, pipefd, &pids[i]))
			return ;
		if (pids[i] == 0)
			exec_child(cmd, mini, prev_pipe, pipefd);
		close_pipe_ends(&prev_pipe, pipefd, cmd->is_piped);
		cmd = cmd->next;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
}

// Main function to execute command list
void	main_executor(t_command *cmd, t_program *mini)
{
	int		count;
	pid_t	*pids;

	count = count_commands(cmd);
	if (count == 1 && cmd->is_builtin)
	{
		exec_loop(cmd, mini, NULL, count);
		return ;
	}
	pids = init_pid_array(count, mini);
	if (!pids)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	exec_loop(cmd, mini, pids, count);
	handle_child_signals(pids, count, mini);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	free(pids);
}
