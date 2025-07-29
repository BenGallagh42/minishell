/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:47:42 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 21:15:10 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handles which built-in to call and returns the exit code
static int	run_builtin_and_get_exit_code(t_command *cmd, t_program *minishell)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd, minishell));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(cmd, minishell));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(cmd, minishell));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(cmd, minishell));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd, minishell));
	return (0);
}

// Executes a built-in command and sets exit code
void	execute_builtin(t_command *cmd, t_program *minishell)
{
	int	exit_code;

	if (!cmd || !cmd->args || !minishell)
		return ;
	exit_code = run_builtin_and_get_exit_code(cmd, minishell);
	if (ft_strcmp(cmd->args[0], "exit") == 0 && !cmd->is_piped)
	{
		minishell->error_code = exit_code;
		exit(exit_code);
	}
	minishell->error_code = exit_code;
	if (cmd->is_piped)
		exit(exit_code);
}
