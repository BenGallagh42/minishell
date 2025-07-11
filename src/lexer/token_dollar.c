/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:35:22 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/11 16:48:35 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_dollar_quotes(const char **input,
	t_token **head, t_token **curr)
{
	const char	*start;
	char		*token;

	start = ++(*input);
	while (**input && **input != '"')
		(*input)++;
	if (**input == '"')
		(*input)++;
	token = ft_strndup(start - 2, *input - start + 2);
	if (!token)
		return ;
	add_token(head, curr, TKN_WORD, token);
}

static void	handle_dollar_question(const char **input,
	t_token **head, t_token **curr)
{
	(*input)++;
	add_token(head, curr, TKN_WORD, ft_strdup("$?"));
}

static void	handle_invalid_var(const char **input,
	t_token **head, t_token **curr)
{
	printf("minishell: syntax error: invalid variable name\n");
	add_token(head, curr, TKN_WORD, ft_strdup("$"));
	(*curr)->error = 1;
	if (**input)
		(*input)++;
}

void	token_dollar(const char **input, t_token **head, t_token **curr)
{
	const char	*start;

	start = *input;
	(*input)++;
	if (**input == '"')
		return (handle_dollar_quotes(input, head, curr));
	if (**input == '?')
		return (handle_dollar_question(input, head, curr));
	if (!ft_isalpha(**input) && **input != '_')
		return (handle_invalid_var(input, head, curr));
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	add_token(head, curr, TKN_WORD, ft_strndup(start, *input - start));
}
