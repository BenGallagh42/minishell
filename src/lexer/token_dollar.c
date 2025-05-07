/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:35:22 by bboulmie          #+#    #+#             */
/*   Updated: 2025/04/22 18:24:51 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	token_dollar(const char **input, t_token **head, t_token **curr)
{
	const char	*start;

	(*input)++;
	if (**input == '?')
	{
		(*input)++;
		add_token(head, curr, TKN_STATUS, ft_strdup("$?"));
	}
	else
	{
		start = *input;
		while (**input && (ft_isalnum(**input) || **input == '_'))
			(*input)++;
		if (*input > start)
			add_token(head, curr, TKN_ENV, ft_strndup(start, *input - start));
		else
			add_token(head, curr, TKN_WORD, "$");
	}
}
