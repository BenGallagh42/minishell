/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:47:42 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/25 12:55:12 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Executes a built-in command and sets exit code
void execute_builtin(t_command *cmd, t_program *minishell)
{
    int exit_code;

    if (!cmd || !cmd->args || !minishell)
        return;
    exit_code = 0;
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        exit_code = ft_echo(cmd);
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
        exit_code = ft_cd(cmd, minishell);
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
        exit_code = ft_pwd();
    else if (ft_strcmp(cmd->args[0], "export") == 0)
        exit_code = ft_export(cmd, minishell);
    else if (ft_strcmp(cmd->args[0], "unset") == 0)
        exit_code = ft_unset(cmd, minishell);
    else if (ft_strcmp(cmd->args[0], "env") == 0)
        exit_code = ft_env(cmd, minishell);
    else if (ft_strcmp(cmd->args[0], "exit") == 0)
    {
        exit_code = ft_exit(cmd, minishell);
        if (!cmd->is_piped)
        {
            minishell->error_code = exit_code;
            exit(exit_code);
        }
    }
    minishell->error_code = exit_code;
    if (cmd->is_piped)
        exit(exit_code);
}
