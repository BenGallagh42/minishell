/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:47:42 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/09 20:38:59 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_command *cmd, t_program *minishell)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		minishell->error_code = ft_echo(cmd);
	// else if (ft_strcmp(cmd->args[0], "cd") == 0)
	// 	minishell->error_code = ft_cd(cmd->args, minishell);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		minishell->error_code = ft_pwd();
	// else if (ft_strcmp(cmd->args[0], "export") == 0)
	// 	minishell->error_code = ft_export(cmd->args, minishell);
	// else if (ft_strcmp(cmd->args[0], "unset") == 0)
	// 	minishell->error_code = ft_unset(cmd->args, minishell);
	// else if (ft_strcmp(cmd->args[0], "env") == 0)
	// 	minishell->error_code = ft_env(minishell->envp);
	// else if (ft_strcmp(cmd->args[0], "exit") == 0)
	// 	ft_exit(cmd->args, minishell);
}
