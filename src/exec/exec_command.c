/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:28:05 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/29 16:43:56 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Executes command with error handling
static void	exec_command_fallback(char *cmd_path,
	t_command *cmd, t_program *mini)
{
	execve(cmd_path, cmd->args, mini->envp);
	mini->error_code = ERR_NO_COMMAND;
	print_error_message(mini->error_code, cmd->args[0], mini);
	free(cmd_path);
	free_command(cmd);
	exit(127);
}

// Handles case when command is not found
static void	handle_cmd_not_found(t_command *cmd, t_program *mini)
{
	print_error_message(mini->error_code, cmd->args[0], mini);
	free_command(cmd);
	if (mini->error_code == ERR_PERMISSION_DENIED)
		exit(126);
	exit(127);
}

// Handles missing command arguments and exits
void	handle_missing_args_and_exit(t_command *cmd, t_program *mini)
{
	mini->error_code = ERR_NO_COMMAND;
	if (cmd->args)
		print_error_message(mini->error_code, cmd->args[0], mini);
	else
		print_error_message(mini->error_code, "", mini);
	free_command(cmd);
	exit(127);
}

// Validates and executes a command
void	validate_and_exec_command(t_command *cmd, t_program *mini)
{
	char	*cmd_path;

	if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
		handle_missing_args_and_exit(cmd, mini);
	cmd_path = find_command_path(cmd->args[0], mini);
	if (!cmd_path)
		handle_cmd_not_found(cmd, mini);
	exec_command_fallback(cmd_path, cmd, mini);
}
