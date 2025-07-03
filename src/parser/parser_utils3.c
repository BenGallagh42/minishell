/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:44:26 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/26 16:02:13 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a NULL-terminated array of strings
void	ft_free_array(char **array)
{
	size_t	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

// Iterates through tokens and chek their error code 
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
