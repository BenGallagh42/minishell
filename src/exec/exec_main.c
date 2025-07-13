/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/10 19:53:04 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Executes all parsed commands, handling piping, redirections, and builtins
void	execute_commands(t_command *cmd, t_program *minishell)
{
	int		prev_pipe = -1;       // File descriptor for the previous pipe's read end
	int		pipefd[2];            // Current pipe file descriptors
	pid_t	*pids;                // Array to store child PIDs
	int		i = 0;
	int		cmd_count = 0;
	int		status;

	// Count commands to allocate PID array
	t_command *temp = cmd;
	while (temp)
	{
		cmd_count++;
		temp = temp->next;
	}

	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		print_error_message(ERR_MEMORY, NULL, minishell);
		return;
	}

	// Execute all commands
	while (cmd)
	{
		// If it's a single builtin, execute in parent to preserve environment
		if (cmd->is_builtin && cmd_count == 1)
		{
			handle_redirections(cmd->redirs, minishell); // Apply redirections in parent
			execute_builtin(cmd, minishell);
			break;
		}

		// Create a pipe if needed
		if (cmd->is_piped && pipe(pipefd) == -1)
		{
			print_error_message(ERR_PIPE, NULL, minishell);
			free(pids);
			return;
		}

		// Fork a new process
		pids[i] = fork();
		if (pids[i] == -1)
		{
			print_error_message(ERR_FORK, NULL, minishell);
			free(pids);
			return;
		}

		// Child process
		if (pids[i] == 0)
		{
			if (prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (cmd->is_piped)
			{
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			handle_redirections(cmd->redirs, minishell);
			if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
			{
				ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
				exit(127); // Standard exit code for "command not found"
			}
			char *cmd_path = find_command_path(cmd->args[0], minishell);
			// /ls error handling fix
			if (!cmd_path)
			{
				if (ft_strchr(cmd->args[0], '/'))
				{
					ft_putstr_fd("minishell: ", STDERR_FILENO);
					ft_putstr_fd(cmd->args[0], STDERR_FILENO);
					ft_putendl_fd(": No such file or directory", STDERR_FILENO);
					exit(127);
				}
				else
				{
					print_error_message(ERR_NO_COMMAND, cmd->args[0], minishell);
					exit(127);
				}
			}
			execve(cmd_path, cmd->args, minishell->envp);
			print_error_message(ERR_PERMISSION_DENIED, cmd->args[0], minishell);
			free(cmd_path);
			exit(126);
		}

		// Parent process: close pipe ends and move to next command
		if (prev_pipe != -1)
			close(prev_pipe);
		if (cmd->is_piped)
		{
			close(pipefd[1]);
			prev_pipe = pipefd[0];
		}
		else
			prev_pipe = -1;

		cmd = cmd->next;
		i++;
	}

	// Close last pipe read end if it exists
	if (prev_pipe != -1)
		close(prev_pipe);

	// Wait for all child processes and capture their exit status
	for (int j = 0; j < i; j++)
	{
		waitpid(pids[j], &status, 0);
		if (WIFEXITED(status))
			minishell->error_code = WEXITSTATUS(status);
	}
	free(pids);
}
