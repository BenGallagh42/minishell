/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:53:59 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:54:03 by bboulmie         ###   ########.fr       */
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
