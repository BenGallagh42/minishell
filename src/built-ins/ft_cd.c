/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:46:37 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/17 21:58:03 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Gets new PWD
static char	*get_new_pwd(void)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_putstr_fd("minishell: cd: getcwd: cannot access parent directories:",
			STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (new_pwd);
}

// Gets target directory for cd
char	*get_cd_target(t_command *cmd, t_program *minishell)
{
	if (!cmd || !cmd->args || !minishell || !minishell->envp)
		return (NULL);
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "--") == 0)
		return (ft_getenv("HOME", minishell->envp));
	if (ft_strcmp(cmd->args[1], "-") == 0)
		return (ft_getenv("OLDPWD", minishell->envp));
	return (cmd->args[1]);
}

// Updates PWD and OLDPWD
int	update_pwd(char *old_pwd, t_program *minishell)
{
	char	*new_pwd;

	if (!old_pwd || !minishell)
	{
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	new_pwd = get_new_pwd();
	if (!new_pwd)
	{
		free(old_pwd);
		return (1);
	}
	if (ft_setenv("OLDPWD", old_pwd, minishell) != 0
		|| ft_setenv("PWD", new_pwd, minishell) != 0)
	{
		free(old_pwd);
		free(new_pwd);
		return (1);
	}
	free(old_pwd);
	free(new_pwd);
	return (0);
}

// Changes directory and updates envp
int	ft_cd(t_command *cmd, t_program *minishell)
{
	char	*target;
	char	*old_pwd;

	if (!cmd || !minishell)
		return (1);
	if (check_cd_errors(cmd, &old_pwd, minishell))
		return (1);
	target = get_cd_target(cmd, minishell);
	if (!target)
	{
		if (cmd->args[1])
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		else
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
		ft_putendl_fd(target, STDOUT_FILENO);
	return (change_directory(target, old_pwd, minishell));
}
