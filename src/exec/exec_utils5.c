/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 02:04:44 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/27 02:41:33 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
				write(STDOUT_FILENO, "Quit\n", 5);
			if (WIFEXITED(status))
				mini->error_code = WEXITSTATUS(status);
		}
	}
}
