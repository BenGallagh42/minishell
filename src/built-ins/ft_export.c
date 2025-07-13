/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:14:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/11 22:42:01 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	handle_invalid_identifier(const char *arg, char *key, char *value)
{
	ft_putstr_fd("export: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
	free(key);
	free(value);
	return (1);
}

static int	process_export_arg(const char *arg, t_program *minishell)
{
	char	*key;
	char	*value;
	int		ret;

	key = NULL;
	value = NULL;
	if (get_key_value(arg, &key, &value))
	{
		free(key);
		free(value);
		return (1);
	}
	if (!is_valid_identifier(key))
		return (handle_invalid_identifier(arg, key, value));
	if (ft_setenv(key, value, minishell) != 0)
		ret = 1;
	else
		ret = 0;
	free(key);
	free(value);
	return (ret);
}

int	ft_export(t_command *cmd, t_program *minishell)
{
	int		i;
	int		ret;
	char	**sorted_env;

	ret = 0;
	if (cmd->args[1] == NULL)
	{
		sorted_env = sort_env(minishell->envp);
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
