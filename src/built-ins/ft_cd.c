/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:46:37 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/10 17:55:01 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_command *cmd, t_program *minishell)
{
	char	*target;
	char	*old_pwd;
	char	*new_pwd;

	if (cmd->args[1] != NULL && cmd->args[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	old_pwd = getcwd(NULL, 0);
	if (old_pwd == NULL)
	{
		ft_putstr_fd("cd: error getting current directory\n", STDERR_FILENO);
		return (1);
	}
	if (cmd->args[1] == NULL)
	{
		target = ft_getenv("HOME", minishell->envp);
		if (target == NULL)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			free(old_pwd);
			return (1);
		}
	}
	else
	{
		target = cmd->args[1];
	}
	if (chdir(target) == -1)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(target, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd == NULL)
	{
		ft_putstr_fd("cd: error getting new current directory\n", STDERR_FILENO);
		free(old_pwd);
		return (1);
	}
	ft_setenv("OLDPWD", old_pwd, minishell);
	ft_setenv("PWD", new_pwd, minishell);
	free(old_pwd);
	free(new_pwd);
	return (0);
}
