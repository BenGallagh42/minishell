/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:27:48 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/10 16:30:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_error(t_program *minishell)
{
	ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
	minishell->error_code = 1;
}

int	ft_env(t_command *cmd, t_program *minishell)
{
	char	**env;

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
	minishell->error_code = 0;
	return (0);
}
