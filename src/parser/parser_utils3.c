/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:44:26 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/26 20:36:58 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a NULL-terminated array of strings
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
