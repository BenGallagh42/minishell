/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 21:08:55 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/07 18:26:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_prompt(char quote)
{
	if (quote == '"')
		return ("dquote> ");
	return ("quote> ");
}

static void	append_new_input(char **input_buf, char quote, t_program *minishell)
{
	char	*prompt;
	char	*new_input;
	char	*temp;

	prompt = get_prompt(quote);
	new_input = readline(prompt);
	if (!new_input)
	{
		printf("minishell: unable to find matching `%c'\n", quote);
		free(*input_buf);
		*input_buf = NULL;
		minishell->error_code = 2;
		return ;
	}
	temp = ft_strjoin(*input_buf, "\n");
	free(*input_buf);
	*input_buf = ft_strjoin(temp, new_input);
	free(temp);
	free(new_input);
	minishell->input_buffer = *input_buf;
}

static void	scan_for_quote(const char **input, char quote, char **full_input,
	t_program *minishell)
{
	const char	*start;
	int			in_double_quotes;

	start = *input;
	in_double_quotes = (quote == '"');
	while (**input)
	{
		if (**input == quote && (in_double_quotes == 0 || (*input == start
					|| *(*input - 1) != '\\')))
			break ;
		(*input)++;
	}
	if (!**input)
	{
		append_new_input(full_input, quote, minishell);
		if (*full_input)
		{
			*input = *full_input + (*input - start);
			scan_for_quote(input, quote, full_input, minishell);
		}
	}
}

void	token_quotes(const char **input, t_token **head,
				t_token **curr, t_program *minishell)
{
	char			quote;
	const char		*start;
	char			*full_input;

	quote = **input;
	start = *input;
	(*input)++;
	full_input = ft_strdup(start);
	scan_for_quote(input, quote, &full_input, minishell);
	if (full_input)
	{
		add_token(head, curr, TKN_WORD, ft_strndup(start, *input - start + 1));
		(*input)++;
		free(full_input);
	}
}
