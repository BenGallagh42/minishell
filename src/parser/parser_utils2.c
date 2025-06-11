/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:01:07 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/10 17:35:55 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a partially built array
void	*free_array_partial(char **array, size_t i)
{
	while (i > 0)
		free(array[--i]);
	free(array);
	return (NULL);
}

// Expands a variable from the input string
void	expand_single_variable(const char **str, char **result,
	t_program *minishell)
{
	const char	*start;
	char		*var_name;
	char		*var_value;

	(*str)++;
	start = *str;
	while (ft_isalnum(**str) || **str == '_')
		(*str)++;
	var_name = ft_substr(start, 0, *str - start);
	if (!var_name)
		return ;
	var_value = ft_getenv(var_name, minishell->envp);
	if (var_value)
	{
		*result = ft_strjoin_free(*result, var_value);
	}
	free(var_name);
}

// Handles unquoted characters and variables
int	process_unquoted(const char **ptr, char **result, t_program *minishell)
{
	if (**ptr == '$')
	{
		expand_single_variable(ptr, result, minishell);
		return (1);
	}
	else if (!append_literal(result, *ptr, 1))
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	(*ptr)++;
	return (1);
}

// Joins two strings and frees the first one
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

// Retrieves the value of an environment variable
char	*ft_getenv(const char *name, char **envp)
{
	size_t	len;
	size_t	i;

	if (name == NULL || envp == NULL)
		return (NULL);
	len = strlen(name);
	i = 0;
	while (envp[i] != NULL)
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}
