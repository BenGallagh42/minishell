/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:49:34 by bboulmie          #+#    #+#             */
/*   Updated: 2025/04/22 16:06:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	token_operator(const char **input, t_token **head, t_token **current)
{
	const char		*start;
	t_token_type	type;

	start = *input;
	if (**input == '|' && *(*input + 1) == '|')
	{
		type = TKN_OR;
		*input += 2;
	}
	else if (**input == '|')
	{
		type = TKN_PIPE;
		(*input)++;
	}
	else if (**input == '&' && *(*input + 1) == '&')
	{
		type = TKN_AND;
		*input += 2;
	}
	else
	{
		type = TKN_BG;
		(*input)++;
	}
	add_token(head, current, type, ft_strndup(start, *input - start));
}
