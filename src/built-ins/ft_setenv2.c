/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:43:11 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/11 21:59:32 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Find index of variable in envp
int	find_env_index(const char *name, char **envp)
{
	size_t	len;
	int		i;

	len = ft_strlen(name); /* get name length */
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=') /* match name and '=' */
			return (i);
		i++;
	}
	return (-1);
}

// Create "name=value" string
char	*create_new_var(const char *name, const char *value)
{
	char	*new_var;
	size_t	name_len;
	size_t	value_len;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_var = malloc(name_len + value_len + 2); /* allocate string */
	if (!new_var)
		return (NULL);
	ft_memcpy(new_var, name, name_len); /* copy name */
	new_var[name_len] = '=';
	ft_memcpy(new_var + name_len + 1, value, value_len); /* copy value */
	new_var[name_len + value_len + 1] = '\0';
	return (new_var);
}

// Allocate new envp array
char	**alloc_new_envp(int n)
{
	char	**new_envp;

	new_envp = malloc(sizeof(char *) * (n + 2)); /* allocate larger array */
	if (!new_envp)
		return (NULL);
	return (new_envp);
}

// Copy old envp and add new_var
void	copy_and_add_to_envp(char **new_envp, char **old_envp,
			char *new_var, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		new_envp[i] = old_envp[i]; /* copy old entries */
		i++;
	}
	new_envp[n] = new_var; /* add new entry */
	new_envp[n + 1] = NULL; /* end array */
}
