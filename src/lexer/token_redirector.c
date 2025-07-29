/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:38:25 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:07:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_error(char redirector, char next_char, int count)
{
	if (count > 2)
	{
		if (redirector == '<')
			ft_printf("minishell: syntax error near unexpected token "
				"`newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `>'\n");
	}
	else if ((redirector == '<' && next_char == '>')
		|| (redirector == '>' && next_char == '<'))
	{
		if (redirector == '<')
			ft_printf("minishell: syntax error near unexpected token "
				"`newline'\n");
		else
			ft_printf("minishell: syntax error near unexpected token `<'\n");
	}
}

// Checks for valid redirection syntax
static int	token_redirector_checker(const char **input)
{
	const char	*start;
	char		redirector;
	int			count;
	char		next_char;

	start = *input;
	redirector = **input;
	count = 0;
	next_char = **input;
	while (**input == redirector)
	{
		count++;
		(*input)++;
	}
	if (count > 2 || ((redirector == '<' && next_char == '>')
			|| (redirector == '>' && next_char == '<')))
	{
		print_error(redirector, next_char, count);
		*input = start;
		return (1);
	}
	*input = start;
	return (0);
}

// Tokenizes redirection operators (<, >, <<, >>)
void	token_redirector(const char **input, t_token **head, t_token **current)
{
	const char		*start;
	t_token_type	type;

	start = *input;
	if (**input == '>' && *(*input + 1) == '>')
	{
		type = TKN_REDIR_APPEND;
		*input += 2;
	}
	else if (**input == '>')
	{
		type = TKN_REDIR_OUT;
		(*input)++;
	}
	else if (**input == '<' && *(*input + 1) == '<')
	{
		type = TKN_REDIR_HEREDOC;
		*input += 2;
	}
	else
	{
		type = TKN_REDIR_IN;
		(*input)++;
	}
	add_token(head, current, type, ft_strndup(start, *input - start));
}

// Handles redirection tokens and checks for syntax errors
void	token_redirector_main(const char **input, t_token **hd, t_token **curr)
{
	const char	*start;
	const char	*next;

	start = *input;
	if (token_redirector_checker(input) == 0)
	{
		token_redirector(input, hd, curr);
		next = *input;
		while (*next && ft_isspace(*next))
			next++;
		if (*next == '<' || *next == '>' || *next == '|' || *next == '&')
		{
			printf("minishell: syntax error");
			(*curr)->error = 1;
		}
	}
	else
	{
		while (**input == '<' || **input == '>')
			(*input)++;
		add_token(hd, curr, TKN_WORD, ft_strndup(start, *input - start));
	}
}
