/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:09:45 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 21:15:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the argument is a valid -n flag
static int	is_valid_n_flag(const char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// Prints echo command arguments
static void	echo_print(char **args, int start, int suppress_newline)
{
	int	i;
	int	first_arg;

	i = start;
	first_arg = 1;
	while (args[i])
	{
		if (!first_arg)
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(args[i], STDOUT_FILENO);
		first_arg = 0;
		i++;
	}
	if (!suppress_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
}

// Executes the echo built-in command
int	ft_echo(t_command *command)
{
	int	suppress_newline;
	int	i;

	i = 1;
	suppress_newline = 0;
	while (command->args[i] && is_valid_n_flag(command->args[i]))
	{
		suppress_newline = 1;
		i++;
	}
	if (!command->args[i] && !suppress_newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	else
		echo_print(command->args, i, suppress_newline);
	return (0);
}
