/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:53:59 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/30 16:48:23 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees an array of strings
void	ft_free_array(char **array)
{
	size_t	i;

	if (!array)
	{
		return ;
	}
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

// Checks tokens for errors
int	has_token_errors(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->error)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

// Sets a command to run in the background
void	handle_background(t_token **tokens, t_command *cmd)
{
	if (*tokens && (*tokens)->type == TKN_BG)
	{
		cmd->is_background = 1;
		*tokens = (*tokens)->next;
	}
}
