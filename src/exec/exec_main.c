/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/27 00:32:32 by hnithyan         ###   ########.fr       */
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

static int	apply_redirections(t_redirection *redirs, t_program *mini)
{
	int				fd;
	static int		heredoc_count;
	t_heredoc_ctx	ctx;

	ctx.fd = &fd;
	ctx.heredoc_count = &heredoc_count;
	return (process_redirection_list(redirs, mini, &ctx));
}

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
			{
				mini->error_code = ERR_MEMORY;
				print_error_message(ERR_MEMORY, NULL, mini);
				return ;
			}
			if (!apply_redirections(cmd->redirs, mini))
			{
				dup2(stdout_backup, STDOUT_FILENO);
				close(stdout_backup);
				return ;
			}
			execute_builtin(cmd, mini);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdout_backup);
			return ;
		}
		if (cmd->is_piped && pipe(pipefd) == -1)
		{
			mini->error_code = ERR_PIPE;
			print_error_message(ERR_PIPE, NULL, mini);
			return ;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			mini->error_code = ERR_FORK;
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
	int			status;
	int			j;

	count = 0;
	tmp = cmd;
	while (tmp && ++count)
		tmp = tmp->next;
	if (count == 1 && cmd->is_builtin)
	{
		exec_loop(cmd, minishell, NULL, count);
		return ;
	}
	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
	{
		print_error_message(ERR_MEMORY, NULL, minishell);
		return ;
	}
	ft_memset(pids, 0, sizeof(pid_t) * count);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	exec_loop(cmd, minishell, pids, count);
	j = -1;
	while (++j < count)
	{
		if (pids[j] > 0)
		{
			waitpid(pids[j], &status, 0);
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
				write(STDOUT_FILENO, "Quit\n", 5);
			if (WIFEXITED(status))
				minishell->error_code = WEXITSTATUS(status);
		}
	}
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	free(pids);
}
