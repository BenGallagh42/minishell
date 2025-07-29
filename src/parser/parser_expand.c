/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:35:25 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:35:27 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Adds a plain text piece to the result
int	append_literal(char **result, const char *str, size_t len)
{
	char	*tmp;
	char	*new;

	tmp = ft_substr(str, 0, len);
	if (!tmp)
		return (0);
	new = ft_strjoin(*result, tmp);
	free(tmp);
	if (!new)
		return (0);
	free(*result);
	*result = new;
	return (1);
}

// Handles single-quoted text without changes
static int	process_single_quotes(const char **str, char **result)
{
	const char	*start;

	(*str)++;
	start = *str;
	while (**str && **str != '\'')
		(*str)++;
	if (**str != '\'')
		return (0);
	if (!append_literal(result, start, *str - start))
		return (0);
	(*str)++;
	return (1);
}

// Handles double-quoted text with variable replacement
static int	process_double_quotes(const char **str, char **result,
	t_program *minishell)
{
	const char	*start;

	(*str)++;
	start = *str;
	while (**str && **str != '"')
	{
		if (**str == '$')
		{
			if (!append_literal(result, start, *str - start))
				return (0);
			expand_single_variable(str, result, minishell);
			start = *str;
		}
		else
			(*str)++;
	}
	if (!append_literal(result, start, *str - start))
		return (0);
	if (**str == '"')
		(*str)++;
	return (1);
}

// Processes the next piece of input text
static int	process_next_token(const char **ptr, char **result,
	t_program *minishell)
{
	if (**ptr == '\'')
	{
		if (!process_single_quotes(ptr, result))
			return (0);
	}
	else if (**ptr == '"')
	{
		if (!process_double_quotes(ptr, result, minishell))
			return (0);
	}
	else
	{
		if (!process_unquoted(ptr, result, minishell))
			return (0);
	}
	return (1);
}

// Replaces variables and removes quotes from text
char	*expand_and_remove_quotes(const char *str, t_program *minishell)
{
	char		*result;
	const char	*ptr;

	result = ft_strdup("");
	if (!result)
	{
		print_error_message(ERR_MEMORY, NULL, minishell);
		return (NULL);
	}
	ptr = str;
	while (*ptr)
	{
		if (!process_next_token(&ptr, &result, minishell))
		{
			print_error_message(ERR_SYNTAX_TOKEN,
				"unbalanced quotes", minishell);
			free(result);
			return (NULL);
		}
	}
	return (result);
}
