/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:28:05 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/29 21:20:33 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Executes command with error handling
static void	exec_command_fallback(char *cmd_path,
	t_command *cmd, t_program *mini)
{
	if (access(cmd_path, X_OK) != 0)
	{
		mini->error_code = ERR_PERMISSION_DENIED;
		print_error_message(mini->error_code, cmd->args[0], mini);
		free(cmd_path);
		free_command(cmd);
		exit(126);
	}
	execve(cmd_path, cmd->args, mini->envp);
	mini->error_code = ERR_PERMISSION_DENIED;
	print_error_message(mini->error_code, cmd->args[0], mini);
	free(cmd_path);
	free_command(cmd);
	exit(126);
}

// Handles case when command is not found
static void	handle_cmd_not_found(t_command *cmd, t_program *mini)
{
	mini->error_code = ERR_NO_COMMAND;
	print_error_message(mini->error_code, cmd->args[0], mini);
	free_command(cmd);
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
	if (cmd->args[0][0] == '/' || ft_strncmp(cmd->args[0], "./", 2) == 0)
	{
		if (access(cmd->args[0], F_OK) == 0 && access(cmd->args[0], X_OK) != 0)
		{
			mini->error_code = ERR_PERMISSION_DENIED;
			print_error_message(mini->error_code, cmd->args[0], mini);
			free_command(cmd);
			exit(126);
		}
		cmd_path = ft_strdup(cmd->args[0]);
	}
	else
		cmd_path = find_command_path(cmd->args[0], mini);
	if (!cmd_path)
		handle_cmd_not_found(cmd, mini);
	exec_command_fallback(cmd_path, cmd, mini);
}
