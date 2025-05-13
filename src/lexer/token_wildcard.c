/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:24:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/13 19:33:14 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '&' || c == '\''
		|| c == '"');
}

int	has_wildcard(const char *input)
{
	while (*input && !ft_isspace(*input) && !is_special(*input))
	{
		if (*input == '*')
			return (1);
		input++;
	}
	return (0);
}

void	token_wildcard(const char **input, t_token **head, t_token **current)
{
	const char	*start;

	start = *input;
	while (**input && !ft_isspace(**input) && !is_special(**input))
		(*input)++;
	add_token(head, current, TKN_WILDCARD, ft_strndup(start, *input - start));
}
