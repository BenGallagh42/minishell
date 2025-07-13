/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:50:39 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/09 20:11:53 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd, t_program *minishell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || ft_strlen(cmd) == 0)
		return (NULL);

	// Case 1: Absolute or relative path -> return if valid
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL); // Don't fallback to PATH if it's a path
	}

	// Case 2: PATH lookup
	path_env = ft_getenv("PATH", minishell->envp);
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

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
