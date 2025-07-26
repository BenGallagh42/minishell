/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:39:45 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/26 23:50:00 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	handle_cmd_not_found(t_command *cmd, t_program *mini)
{
	print_error_message(mini->error_code, cmd->args[0], mini);
	free_command(cmd);
	if (mini->error_code == ERR_PERMISSION_DENIED)
		exit(126);
	exit(127);
}

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
