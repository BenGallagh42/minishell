/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:48:04 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 09:40:13 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_exit(t_command *cmd, t_program *minishell)
{
	int			code;
	int			ret;
	long long	ll_code;

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
		return (print_exit_and_return_code(minishell, cmd->args[1]));
	code = (int)ll_code;
	ft_putendl_fd("exit", STDERR_FILENO);
	minishell->error_code = (unsigned char)code;
	return (minishell->error_code);
}
