/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:28:18 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/05 16:05:59 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Adds a new token to the token list
void	add_token(t_token **head, t_token **curr, t_token_type type, char *val)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = ft_strdup(val);
	new_token->error = 0;
	new_token->next = NULL;
	if (*curr)
		(*curr)->next = new_token;
	else
		*head = new_token;
	*curr = new_token;
}
