/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:50:39 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/25 11:06:02 by bboulmie         ###   ########.fr       */
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

char	*find_command_path(char *cmd, t_program *minishell)
{
	char		*path_env;
	char		**paths;
	struct stat	sb;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
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
	path_env = ft_getenv("PATH", minishell->envp);
	if (!path_env)
	{
		minishell->error_code = ERR_NO_COMMAND;
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
	{
		minishell->error_code = ERR_MEMORY;
		return (NULL);
	}
	path_env = search_path_dirs(cmd, paths);
	if (!path_env)
		minishell->error_code = ERR_NO_COMMAND;
	return (path_env);
}
