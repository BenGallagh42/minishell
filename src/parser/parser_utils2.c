/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:51:36 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:51:39 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a partially built string array
void	*free_array_partial(char **array, size_t i)
{
	while (i > 0)
		free(array[--i]);
	free(array);
	return (NULL);
}

// Replaces a variable in the input string
void	expand_single_variable(const char **str, char **result,
	t_program *minishell)
{
	char	*var_name;

	(*str)++;
	if (is_special_dollar_case(str, result))
		return ;
	var_name = get_variable_name(str);
	if (!var_name)
		return ;
	expand_and_append_value(var_name, result, minishell);
	free(var_name);
}

// Handles text without quotes and replaces variables
int	process_unquoted(const char **ptr, char **result, t_program *minishell)
{
	if (**ptr == '$')
	{
		expand_single_variable(ptr, result, minishell);
		return (1);
	}
	if (!append_literal(result, *ptr, 1))
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	(*ptr)++;
	return (1);
}

// Combines two strings and frees the first
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

// Gets the value of an environment variable
char	*ft_getenv(const char *name, char **envp)
{
	size_t	len;
	size_t	i;

	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}
