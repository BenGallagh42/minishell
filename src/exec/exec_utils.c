/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:50:39 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/26 20:53:10 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_path_dirs(char *cmd, char **paths)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_free(ft_strjoin(paths[i], "/"), cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

static char	*is_valid_absolute_path(char *cmd, t_program *minishell)
{
	struct stat	sb;

	if (stat(cmd, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			minishell->error_code = ERR_PERMISSION_DENIED;
			return (NULL);
		}
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	minishell->error_code = ERR_FILE_NOT_FOUND;
	return (NULL);
}

static char	*handle_null_path_env(t_program *minishell)
{
	minishell->error_code = ERR_NO_COMMAND;
	return (NULL);
}

static char	*handle_split_and_search(char *cmd, char *path_env,
	t_program *minishell)
{
	char	**paths;
	char	*found_path;

	paths = ft_split(path_env, ':');
	if (!paths)
	{
		minishell->error_code = ERR_MEMORY;
		return (NULL);
	}
	found_path = search_path_dirs(cmd, paths);
	if (!found_path)
		minishell->error_code = ERR_NO_COMMAND;
	return (found_path);
}

char	*find_command_path(char *cmd, t_program *minishell)
{
	char	*path_env;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (is_valid_absolute_path(cmd, minishell));
	path_env = ft_getenv("PATH", minishell->envp);
	if (!path_env)
		return (handle_null_path_env(minishell));
	return (handle_split_and_search(cmd, path_env, minishell));
}
