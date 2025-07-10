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

void execute_commands(t_command *cmd, t_program *minishell)
{
    int prev_pipe = -1; // File descriptor for the previous pipe's read end
    int pipefd[2];      // Current pipe file descriptors
    pid_t pid;          // Process ID for forking
    int status;         // Exit status of child processes
    char *cmd_path;
    pid_t *pids = NULL; // Array to store child PIDs
    int cmd_count = 0;  // Number of commands
    int i;

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
        printf("Memory allocation failed\n");
        return;
    }
    i = 0;

    // Execute all commands
    while (cmd)
    {
        if (cmd->is_builtin)
        {
            execute_builtin(cmd, minishell);
        }
        else
        {
            if (cmd->is_piped)
            {
                if (pipe(pipefd) == -1)
                {
                    printf("Pipe failed\n");
                    free(pids);
                    return;
                }
            }

            pid = fork();
            if (pid == -1)
            {
                printf("Fork failed\n");
                free(pids);
                return;
            }

            if (pid == 0) // Child process
            {
                if (prev_pipe != -1)
                {
                    dup2(prev_pipe, STDIN_FILENO);
                    close(prev_pipe);
                }
                if (cmd->is_piped)
                {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[0]);
                    close(pipefd[1]);
                }
                handle_redirections(cmd->redirs, minishell);
                cmd_path = find_command_path(cmd->args[0], minishell);
                if (!cmd_path)
                {
                    print_error_message(ERR_NO_COMMAND, cmd->args[0], minishell);
                    exit(127);
                }
                execve(cmd_path, cmd->args, minishell->envp);
                print_error_message(ERR_PERMISSION_DENIED, cmd->args[0], minishell);
                free(cmd_path);
                exit(126);
            }
            else // Parent process
            {
                pids[i++] = pid; // Store PID
                if (prev_pipe != -1)
                    close(prev_pipe);
                if (cmd->is_piped)
                {
                    close(pipefd[1]);
                    prev_pipe = pipefd[0];
                }
                else
                {
                    prev_pipe = -1;
                }
            }
        }
        cmd = cmd->next;
    }

    // Close last pipe read end if it exists
    if (prev_pipe != -1)
        close(prev_pipe);

    // Wait for all child processes
    for (int j = 0; j < i; j++)
    {
        waitpid(pids[j], &status, 0);
        if (WIFEXITED(status))
            minishell->error_code = WEXITSTATUS(status);
    }
    free(pids);
}
