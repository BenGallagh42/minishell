/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:00:56 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 19:00:58 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees the shell’s environment variables
void	free_shell(t_program *minishell)
{
	int	i;

	i = 0;
	while (minishell->envp[i])
		free(minishell->envp[i++]);
	free(minishell->envp);
}

// Frees a list of tokens
void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}
