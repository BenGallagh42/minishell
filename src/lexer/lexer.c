/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:20:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/24 20:53:48 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_dollar_token(const char **input, t_token **head,
	t_token **current, t_program *minishell)
{
	token_dollar(input, head, current, minishell);
}

static void	process_lexer_token(const char **input, t_token **head,
	t_token **current, t_program *minishell)
{
	if (**input == '<' || **input == '>')
		token_redirector_main(input, head, current);
	else if (**input == '|' || **input == '&')
		token_operator(input, head, current);
	else if (**input == '$')
		handle_dollar_token(input, head, current, minishell);
	else if (has_wildcard(*input))
		token_wildcard(input, head, current);
	else
		token_word(input, head, current);
}

static int	is_only_spaces(const char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}

static void	concatenate_quoted_string(const char **input, char **result,
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
			ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
			ft_putchar_fd(quote, 2);
			ft_putstr_fd("'\n", 2);
			minishell->error_code = 2;
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

t_token	*main_lexer(const char *input, t_program *minishell)
{
	t_token	*head;
	t_token	*current;
	char	*quoted_str;

	head = NULL;
	current = NULL;
	if (!input || !*input || is_only_spaces(input))
		return (NULL);
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (!*input || *input == '#')
			break ;
		if (*input == '\'' || *input == '"')
		{
			quoted_str = ft_strdup("");
			if (!quoted_str)
				return (NULL);
			concatenate_quoted_string(&input, &quoted_str, minishell);
			if (minishell->error_code != 0 || !quoted_str)
			{
				free(quoted_str);
				break ;
			}
			if (*quoted_str)
				add_token(&head, &current, TKN_WORD, quoted_str);
			else
				free(quoted_str);
		}
		else
			process_lexer_token(&input, &head, &current, minishell);
	}
	return (head);
}
