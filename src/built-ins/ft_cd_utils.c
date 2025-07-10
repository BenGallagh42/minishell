/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:06 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/10 19:06:39 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_index(const char *name, char **envp)
{
	size_t	len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	ft_setenv(const char *name, const char *value, t_program *minishell)
{
	size_t	name_len;
	size_t	value_len;
	char	*new_var;
	int		index;
	int		n;

	if (name == NULL || strchr(name, '=') != NULL)
		return (-1);
	if (value == NULL)
		value = "";
	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
		return (-1);
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_memcpy(new_var + name_len + 1, value, value_len);
	new_var[name_len + value_len + 1] = '\0';
	index = find_env_index(name, minishell->envp);
	if (index != -1)
	{
		free(minishell->envp[index]);
		minishell->envp[index] = new_var;
	}
	else
	{
		n = 0;
		while (minishell->envp && minishell->envp[n])
			n++;
		char **new_envp = malloc(sizeof(char *) * (n + 2));
		if (!new_envp)
		{
			free(new_var);
			return (-1);
		}
		for (int i = 0; i < n; i++)
		new_envp[i] = minishell->envp[i];
		new_envp[n] = new_var;
		new_envp[n + 1] = NULL;
		free(minishell->envp);
		minishell->envp = new_envp;
	}
	return (0);
}
