/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 19:09:24 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/17 19:10:19 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redirection(t_token **current, t_program *minishell)
{
	if (!(*current)->next || !is_word_token((*current)->next->type))
	{
		print_syntax_error((*current)->value, minishell);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}

int	process_token(t_token **current, int *has_command, t_program *minishell)
{
	if (is_word_token((*current)->type))
	{
		*has_command = 1;
	}
	else if (is_redirection_token((*current)->type))
	{
		if (!handle_redirection(current, minishell))
			return (0);
	}
	else if ((*current)->type == TKN_PIPE)
	{
		if (!*has_command)
		{
			print_syntax_error("|", minishell);
			return (0);
		}
		*has_command = 0;
	}
	else
	{
		print_syntax_error((*current)->value, minishell);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}
