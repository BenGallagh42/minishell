/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:36:47 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:36:48 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handles special dollar sign cases
int	is_special_dollar_case(const char **str, char **result)
{
	char	*tmp;

	if (**str == '\0' || **str == ' ' || **str == '"' || **str == '\'')
	{
		tmp = ft_strdup("$");
		if (!tmp)
			return (1);
		*result = ft_strjoin_free(*result, tmp);
		free(tmp);
		return (1);
	}
	return (0);
}

// Gets the variable name from input
char	*get_variable_name(const char **str)
{
	const char	*start;
	char		*var_name;

	if (**str == '?')
	{
		var_name = ft_strdup("?");
		(*str)++;
	}
	else
	{
		start = *str;
		while (ft_isalnum(**str) || **str == '_')
			(*str)++;
		var_name = ft_substr(start, 0, *str - start);
	}
	return (var_name);
}

// Replaces a variable with its value
void	expand_and_append_value(char *var_name, char **result,
	t_program *minishell)
{
	char	*var_value;

	if (ft_strcmp(var_name, "?") == 0)
	{
		var_value = ft_itoa(minishell->error_code);
		if (var_value)
		{
			*result = ft_strjoin_free(*result, var_value);
			free(var_value);
		}
	}
	else
	{
		var_value = ft_getenv(var_name, minishell->envp);
		if (var_value)
			*result = ft_strjoin_free(*result, var_value);
	}
}
