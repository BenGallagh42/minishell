/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/24 18:40:07 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipe_ends(int *prev_pipe, int *pipefd, int is_piped)
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

static void	exec_child(t_command *cmd, t_program *mini, int prev_pipe, int *pipefd)
{
	char	*cmd_path;

	handle_redirections(cmd->redirs, mini);
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
	if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
	{
		mini->error_code = ERR_NO_COMMAND;
		print_error_message(mini->error_code, cmd->args ? cmd->args[0] : "", mini);
		free_command(cmd);
		exit(127);
	}
	cmd_path = find_command_path(cmd->args[0], mini);
	if (!cmd_path)
	{
		print_error_message(mini->error_code, cmd->args[0], mini);
		free_command(cmd);
		if (mini->error_code == ERR_PERMISSION_DENIED)
			exit(126);
		exit(127);
	}
	execve(cmd_path, cmd->args, mini->envp);
	mini->error_code = ERR_NO_COMMAND;
	print_error_message(mini->error_code, cmd->args[0], mini);
	free(cmd_path);
	free_command(cmd);
	exit(127);
}

static void	exec_loop(t_command *cmd, t_program *mini, pid_t *pids, int count)
{
	int	i;
	int	prev_pipe;
	int	pipefd[2];
	int	stdout_backup;

	i = -1;
	prev_pipe = -1;
	while (++i < count)
	{
		if (cmd->is_builtin && count == 1)
		{
			stdout_backup = dup(STDOUT_FILENO);
			if (stdout_backup == -1)
				return (print_error_message(ERR_MEMORY, NULL, mini));
			handle_redirections(cmd->redirs, mini);
			execute_builtin(cmd, mini);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdout_backup);
			return ;
		}
		if (cmd->is_piped && pipe(pipefd) == -1)
		{
			print_error_message(ERR_PIPE, NULL, mini);
			return ;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			print_error_message(ERR_FORK, NULL, mini);
			return ;
		}
		if (pids[i] == 0)
			exec_child(cmd, mini, prev_pipe, pipefd);
		close_pipe_ends(&prev_pipe, pipefd, cmd->is_piped);
		cmd = cmd->next;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
}

void	execute_commands(t_command *cmd, t_program *minishell)
{
	t_command	*tmp;
	int			count;
	pid_t		*pids;
	int			status = 0;
	int			j;

	count = 0;
	tmp = cmd;
	while (tmp && ++count)
		tmp = tmp->next;
	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
	{
		print_error_message(ERR_MEMORY, NULL, minishell);
		return ;
	}
	ft_memset(pids, 0, sizeof(pid_t) * count);
	exec_loop(cmd, minishell, pids, count);
	j = -1;
	while (++j < count)
	{
		if (pids[j] > 0)
		{
			waitpid(pids[j], &status, 0);
			if (WIFEXITED(status))
				minishell->error_code = WEXITSTATUS(status);
		}
	}
	free(pids);
}
