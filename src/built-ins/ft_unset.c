/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:23:50 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 09:44:27 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Removes a variable from envp
static void	remove_var(char **envp, int index)
{
	while (envp[index])
	{
		envp[index] = envp[index + 1];
		index++;
	}
}

// Unsets environment variables
int	ft_unset(t_command *cmd, t_program *minishell)
{
	int		i;
	int		index;

	if (!cmd || !minishell || !cmd->args)
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] && is_valid_identifier(cmd->args[i]))
		{
			index = find_env_index(cmd->args[i], minishell->envp);
			if (index != -1)
				remove_var(minishell->envp, index);
		}
		i++;
	}
	minishell->error_code = 0;
	return (0);
}
