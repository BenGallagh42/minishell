/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:38:25 by bboulmie          #+#    #+#             */
/*   Updated: 2025/04/23 18:34:14 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_error(char redirector, char next_char, int count)
{
	if (count > 2)
	{
		if (redirector == '<')
			printf("minishell: syntax error near unexpected token `newline'\n");
		else
			printf("minishell: syntax error near unexpected token `>'\n");
	}
	else if ((redirector == '<' && next_char == '>')
		|| (redirector == '>' && next_char == '<'))
	{
		if (redirector == '<')
			printf("minishell: syntax error near unexpected token `newline'\n");
		else
			printf("minishell: syntax error near unexpected token `<'\n");
	}
}

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

void	token_redirector_main(const char **input, t_token **hd, t_token **curr)
{
	const char	*start;

	start = *input;
	if (token_redirector_checker(input) == 0)
		token_redirector(input, hd, curr);
	else
	{
		while (**input == '<' || **input == '>')
			(*input)++;
		add_token(hd, curr, TKN_WORD, ft_strndup(start, *input - start));
	}
}
