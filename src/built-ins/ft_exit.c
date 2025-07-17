/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:48:04 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/17 21:51:44 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Handles exit error cases
static int	handle_exit_errors(t_command *cmd, t_program *minishell)
{
	if (!cmd->args[1])
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		minishell->error_code = 0;
		return (0);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDOUT_FILENO);
		ft_putstr_fd(cmd->args[1], STDOUT_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDOUT_FILENO);
		minishell->error_code = 255;
		return (255);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDOUT_FILENO);
		minishell->error_code = 1;
		return (1);
	}
	return (-1);
}

// Exits the shell with specified code
int	ft_exit(t_command *cmd, t_program *minishell)
{
	int		code;
	int		ret;

	if (!cmd || !minishell || !cmd->args)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		minishell->error_code = 1;
		return (1);
	}
	ret = handle_exit_errors(cmd, minishell);
	if (ret != -1)
		return (ret);
	code = ft_atoi(cmd->args[1]);
	if (code > INT_MAX || code < INT_MIN)
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDOUT_FILENO);
		ft_putstr_fd(cmd->args[1], STDOUT_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDOUT_FILENO);
		minishell->error_code = 255;
		return (255);
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	minishell->error_code = (unsigned char)code;
	return (minishell->error_code);
}
