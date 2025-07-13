/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:06 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/11 21:42:06 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Update existing variable
static int update_var(char **envp, char *new_var, int index)
{
	free(envp[index]); /* free old value */
	envp[index] = new_var; /* replace with new */
	return (0);
}

// Add new variable to envp
static int add_new_var(const char *name, const char *value, t_program *minishell)
{
	char	*new_var;
	char	**new_envp;
	int		n;

	if (value == NULL)
		value = ""; /* default empty value */
	new_var = create_new_var(name, value); /* create "name=value" */
	if (!new_var)
		return (-1);
	n = 0;
	while (minishell->envp && minishell->envp[n])
		n++; /* count current envp */
	new_envp = alloc_new_envp(n); /* allocate new array */
	if (!new_envp)
	{
		free(new_var);
		return (-1);
	}
	copy_and_add_to_envp(new_envp, minishell->envp, new_var, n); /* copy and add */
	free(minishell->envp); /* free old array */
	minishell->envp = new_envp; /* update envp */
	return (0);
}

// Set or add environment variable
int ft_setenv(const char *name, const char *value, t_program *minishell)
{
	char	*new_var;
	int		index;

	if (name == NULL || ft_strchr(name, '=') != NULL) /* invalid name */
		return (-1);
	index = find_env_index(name, minishell->envp); /* find if exists */
	new_var = create_new_var(name, value); /* create "name=value" */
	if (!new_var)
		return (-1);
	if (index != -1)
	{
		update_var(minishell->envp, new_var, index); /* update existing */
		return (0);
	}
	else
		return (add_new_var(name, value, minishell)); /* add new */
}
