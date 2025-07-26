/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/27 02:06:00 by hnithyan         ###   ########.fr       */
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

int	apply_redirections(t_redirection *redirs, t_program *mini)
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

void	execute_commands(t_command *cmd, t_program *mini)
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
