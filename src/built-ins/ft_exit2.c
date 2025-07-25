/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:35:03 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/21 20:12:07 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_atoll(const char *str, int *i, int *sign)
{
	*i = 0;
	*sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	init_atoll(str, &i, &sign);
	while (str[i] && ft_isdigit(str[i]))
	{
		if (result > LLONG_MAX / 10
			|| (result == LLONG_MAX / 10 && (str[i] - '0') > LLONG_MAX % 10))
			return (sign == 1 ? LLONG_MAX : LLONG_MIN);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

int	handle_exit_errors(t_command *cmd, t_program *minishell)
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

int	print_exit_and_return_code(t_program *minishell, const char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	minishell->error_code = 2;
	return (2);
}
