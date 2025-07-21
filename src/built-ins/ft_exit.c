/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:48:04 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 16:37:08 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		if (result > LLONG_MAX / 10
			|| (result == LLONG_MAX / 10 && str[i] - '0' > LLONG_MAX % 10))
			return (sign == 1 ? LLONG_MAX : LLONG_MIN);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

static int	handle_exit_errors(t_command *cmd, t_program *minishell)
{
	if (!cmd->args[1])
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		minishell->error_code = 0;
		return (0);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		minishell->error_code = 2;
		return (2);
	}
	if (cmd->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		minishell->error_code = 1;
		return (1);
	}
	return (-1);
}

int	ft_exit(t_command *cmd, t_program *minishell)
{
	int				code;
	int				ret;
	long long		ll_code;

	if (!cmd || !minishell || !cmd->args)
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		minishell->error_code = 1;
		return (1);
	}
	ret = handle_exit_errors(cmd, minishell);
	if (ret != -1)
		return (ret);
	ll_code = ft_atoll(cmd->args[1]);
	if (ll_code > INT_MAX || ll_code < INT_MIN)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		minishell->error_code = 2;
		return (2);
	}
	code = (int)ll_code;
	ft_putendl_fd("exit", STDERR_FILENO);
	minishell->error_code = (unsigned char)code;
	return (minishell->error_code);
}
