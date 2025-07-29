/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:28:18 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 16:18:50 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Adds a new token to the token list
void	add_token(t_token **head, t_token **curr, t_token_type type, char *val)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(val);
		return ;
	}
	new_token->type = type;
	new_token->value = ft_strdup(val);
	if (!new_token->value)
	{
		free(new_token);
		free(val);
		return ;
	}
	new_token->error = 0;
	new_token->next = NULL;
	if (*curr)
		(*curr)->next = new_token;
	else
		*head = new_token;
	*curr = new_token;
	free(val);
}

// Prints syntax error message for unclosed quote
void	print_quote_error(char quote, t_program *minishell)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putchar_fd(quote, 2);
	ft_putstr_fd("'\n", 2);
	minishell->error_code = 2;
}

// Concatenates consecutive quoted strings
void	concatenate_quoted_string(const char **input, char **result,
	t_program *minishell)
{
	char		*tmp;
	const char	*start;
	const char	*end;
	char		quote;

	while (**input == '\'' || **input == '"')
	{
		quote = **input;
		start = *input;
		(*input)++;
		end = scan_for_quote(input, quote);
		if (!end)
		{
			print_quote_error(quote, minishell);
			return ;
		}
		tmp = ft_strndup(start, end - start + 1);
		if (!tmp)
			return ;
		*result = ft_strjoin_free(*result, tmp);
		free(tmp);
		*input = end + 1;
	}
}
