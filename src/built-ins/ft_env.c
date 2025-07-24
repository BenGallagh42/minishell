/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:27:48 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/24 20:21:59 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints env error for invalid command
static void	print_env_error(t_program *minishell, const char *arg)
{
	ft_putstr_fd("env: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	minishell->error_code = 127;
}

// Prints environment variables
int	ft_env(t_command *cmd, t_program *minishell)
{
	char	**env;

	if (!cmd || !minishell || !minishell->envp)
		return (1);
	env = minishell->envp;
	if (cmd->args[1] != NULL)
	{
		print_env_error(minishell, cmd->args[1]);
		return (1);
	}
	while (*env)
	{
		ft_putstr_fd(*env, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		env++;
	}
	minishell->error_code = 0;
	return (0);
}
