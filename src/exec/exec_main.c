/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/09 19:41:10 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_command *cmd, t_program *minishell)
{
	int		prev_pipe; // File descriptor for the previous pipe's end
	int 	pipefd[2];      // Current pipe file descriptors
	pid_t 	pid;          // Process ID for forking
	int 	status;         // Exit status of child processes
	char	*cmd_path;

	prev_pipe = -1;
	while (cmd)
	{
        // Set up pipe if the command is piped to the next
		if (cmd->is_piped)
		{
			if (pipe(pipefd) == -1)
			{
				printf("Pipe failed\n");
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			printf("Fork failed\n");
			return ;
		}
		if (pid == 0) // Child process
		{
			// Handle input from previous pipe
			if (prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			// Handle output to next pipe
			if (cmd->is_piped)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			// Apply redirections
			handle_redirections(cmd->redirs, minishell);
			// Execute command
			if (cmd->is_builtin)
			{
				execute_builtin(cmd, minishell);
				exit(minishell->error_code);
			}
			else
			{
				cmd_path = find_command_path(cmd->args[0], minishell);
				if (!cmd_path)
				{
					print_error_message(ERR_NO_COMMAND, cmd->args[0], minishell);
					exit(127); // Command not found
				}
				execve(cmd_path, cmd->args, minishell->envp);
				print_error_message(ERR_PERMISSION_DENIED, cmd->args[0], minishell);
				free(cmd_path);
				exit(126); // Permission denied or execve failure
			}
		}
		else // Parent process
		{
			// Close previous pipe's read end
			if (prev_pipe != -1)
				close(prev_pipe);
			// Close current pipe's write end and save read end
			if (cmd->is_piped)
			{
				close(pipefd[1]);
				prev_pipe = pipefd[0];
			}
			else
			{
				prev_pipe = -1;
			}
			// Wait for child unless it's a background job
			if (!cmd->is_background)
			{
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					minishell->error_code = WEXITSTATUS(status);
			}
			cmd = cmd->next;
		}
	}
}
