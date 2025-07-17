/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:27:48 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/17 21:42:04 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints env error for too many arguments
static void	print_env_error(t_program *minishell)
{
	ft_putstr_fd("minishell: env: too many arguments\n", STDERR_FILENO);
	minishell->error_code = 1;
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
		print_env_error(minishell);
		return (1);
	}
	while (*env)
	{
		ft_putstr_fd(*env, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		env++;
	}
	return (0);
}
