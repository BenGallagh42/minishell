/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:06 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/23 19:15:46 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Updates existing variable
static int	update_var(char **envp, char *new_var, int index)
{
	if (!envp || !new_var)
		return (1);
	free(envp[index]);
	envp[index] = new_var;
	return (0);
}

// Handles invalid name error
static int	handle_setenv_error(const char *name)
{
	ft_putstr_fd("minishell: export: ", STDERR_FILENO);
	if (name && name[0])
		ft_putstr_fd((char *)name, STDERR_FILENO);
	else
		ft_putstr_fd("''", STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	return (1);
}

// Allocates and copies envp with new variable
static int	alloc_and_copy_envp(char *new_var, t_program *minishell)
{
	char	**new_envp;
	int		n;

	n = 0;
	while (minishell->envp && minishell->envp[n])
		n++;
	new_envp = alloc_new_envp(n);
	if (!new_envp)
	{
		free(new_var);
		return (1);
	}
	copy_and_add_to_envp(new_envp, minishell->envp, new_var, n);
	free(minishell->envp);
	minishell->envp = new_envp;
	return (0);
}

// Sets or adds environment variable
int	ft_setenv(const char *name, const char *value, t_program *minishell)
{
	char	*new_var;
	int		index;

	if (!name || !name[0] || !minishell || ft_strchr(name, '='))
		return (handle_setenv_error(name));
	if (!value)
		value = "";
	new_var = create_new_var(name, value);
	if (!new_var)
	{
		ft_putstr_fd("minishell: export: memory allocation failed\n",
			STDERR_FILENO);
		return (1);
	}
	index = find_env_index(name, minishell->envp);
	if (index != -1)
		return (update_var(minishell->envp, new_var, index));
	return (alloc_and_copy_envp(new_var, minishell));
}
