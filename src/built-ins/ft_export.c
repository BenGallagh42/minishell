/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:14:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:42:13 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Splits arg into key and value
static int	get_key_value(const char *arg, char **key, char **value)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*key = ft_strndup(arg, eq - arg);
		*value = ft_strdup(eq + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (eq && !*value))
	{
		free(*key);
		free(*value);
		return (1);
	}
	return (0);
}

// Handles invalid identifier error
static int	handle_invalid_identifier(const char *arg, char *key, char *value)
{
	ft_putstr_fd("export: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	free(key);
	free(value);
	return (1);
}

// Removes quotes from value
static char	*remove_value_quotes(char *value)
{
	char	*new_value;
	size_t	len;

	new_value = NULL;
	if (!value)
		return (NULL);
	len = ft_strlen(value);
	if (len >= 2 && ((value[0] == '"' && value[len - 1] == '"')
			|| (value[0] == '\'' && value[len - 1] == '\'')))
	{
		new_value = ft_strndup(value + 1, len - 2);
		free(value);
		return (new_value);
	}
	return (value);
}

// Processes one export argument
static int	process_export_arg(const char *arg, t_program *minishell)
{
	char	*key;
	char	*value;
	int		ret;

	key = NULL;
	value = NULL;
	if (get_key_value(arg, &key, &value))
		return (1);
	if (!is_valid_identifier(key))
		return (handle_invalid_identifier(arg, key, value));
	if (value)
		value = remove_value_quotes(value);
	ret = ft_setenv(key, value, minishell);
	free(key);
	free(value);
	if (ret != 0)
	{
		ft_putstr_fd("export: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": error setting variable\n", STDERR_FILENO);
	}
	return (ret);
}

// // Executes the export built-in command
int	ft_export(t_command *cmd, t_program *minishell)
{
	int		i;
	int		ret;
	char	**sorted_env;

	ret = 0;
	if (cmd->args[1] == NULL)
	{
		sorted_env = sort_env(minishell->envp);
		if (!sorted_env)
			return (1);
		print_sorted_env(sorted_env);
		ft_free_array(sorted_env);
		minishell->error_code = 0;
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (process_export_arg(cmd->args[i], minishell) != 0)
			ret = 1;
		i++;
	}
	minishell->error_code = ret;
	return (ret);
}
