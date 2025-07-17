/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:43:56 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/16 20:49:27 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if variable name is valid
int	is_valid_identifier(const char *key)
{
	int	valid;

	valid = 1;
	if (!key || (!ft_isalpha(*key) && *key != '_'))
		valid = 0;
	if (valid == 0)
		return (0);
	while (*key)
	{
		if (!ft_isalnum(*key) && *key != '_')
			return (0);
		key++;
	}
	return (1);
}

// Sorts environment variables
static void	bubble_sort(char **sorted)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (sorted[i])
	{
		j = i + 1;
		while (sorted[j])
		{
			if (ft_strcmp(sorted[i], sorted[j]) > 0)
			{
				temp = sorted[i];
				sorted[i] = sorted[j];
				sorted[j] = temp;
			}
			j++;
		}
		i++;
	}
}

// Creates sorted copy of envp
char	**sort_env(char **envp)
{
	char	**sorted;
	int		len;
	int		i;

	len = 0;
	while (envp[len])
		len++;
	sorted = malloc(sizeof(char *) * (len + 1));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sorted[i] = ft_strdup(envp[i]);
		i++;
	}
	sorted[len] = NULL;
	bubble_sort(sorted);
	return (sorted);
}

// Prints one environment variable in declare format
static void	print_one_env_var(char *env_var)
{
	char	*key;
	char	*value;

	key = env_var;
	value = ft_strchr(env_var, '=');
	if (value)
	{
		*value = '\0';
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(value + 1, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
		*value = '=';
	}
	else
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(key, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
}

// Prints sorted envp in declare format
void	print_sorted_env(char **sorted)
{
	int	i;

	i = 0;
	while (sorted[i])
	{
		print_one_env_var(sorted[i]);
		i++;
	}
}
