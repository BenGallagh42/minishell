/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:24:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/05/07 18:26:20 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	token_wildcard(const char **input, t_token **head, t_token **current)
{
	const char	*start;

	start = *input;
	while (**input == '*')
		(*input)++;
	add_token(head, current, TKN_WILDCARD, ft_strndup(start, *input - start));
}