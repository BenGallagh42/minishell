/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 20:58:39 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/17 21:50:49 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks for cd errors (too many args, getcwd)
int	check_cd_errors(t_command *cmd, char **old_pwd, t_program *minishell)
{
	if (!cmd || !old_pwd || !minishell)
		return (1);
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	*old_pwd = getcwd(NULL, 0);
	if (!*old_pwd)
	{
		ft_putstr_fd("minishell: cd: getcwd: cannot access parent directories:",
			STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

// Changes directory and updates envp
int	change_directory(char *target, char *old_pwd, t_program *minishell)
{
	if (!target || !old_pwd || !minishell)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	if (chdir(target) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free(old_pwd);
		return (1);
	}
	return (update_pwd(old_pwd, minishell));
}
