/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:39:45 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/30 15:56:52 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Closes pipe ends after command execution
void	close_pipe_ends(int *prev_pipe, int *pipefd, int is_piped)
{
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (is_piped)
	{
		close(pipefd[1]);
		*prev_pipe = pipefd[0];
	}
	else
		*prev_pipe = -1;
}

// Counts the number of commands in the list
int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

// Initializes an array for process IDs
pid_t	*init_pid_array(int count, t_program *mini)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
	{
		print_error_message(ERR_MEMORY, NULL, mini);
		return (NULL);
	}
	ft_memset(pids, 0, sizeof(pid_t) * count);
	return (pids);
}

// Handles signal and exit status for a single process
static void	process_child_status(int status, t_program *mini)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
			mini->error_code = 128 + WTERMSIG(status);
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			mini->error_code = 128 + WTERMSIG(status);
		}
	}
	else if (WIFEXITED(status))
		mini->error_code = WEXITSTATUS(status);
}

// Handles signals and exit statuses for child processes
void	handle_child_signals(pid_t *pids, int count, t_program *mini)
{
	int	j;
	int	status;

	j = -1;
	while (++j < count)
	{
		if (pids[j] > 0)
		{
			waitpid(pids[j], &status, 0);
			process_child_status(status, mini);
		}
	}
}
