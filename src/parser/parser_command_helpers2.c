/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helpers2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 19:32:24 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/25 20:10:16 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Expands a tilde (~) to the user's HOME directory in the given string
char	*handle_tilde_expansion(const char *value, t_program *minishell)
{
	char	*home;
	char	*expanded;

	home = ft_getenv("HOME", minishell->envp);
	if (!home)
	{
		print_error_message(ERR_NO_COMMAND, "HOME not set", minishell);
		return (NULL);
	}
	if (value[1] == '\0')
		expanded = ft_strdup(home);
	else
		expanded = ft_strjoin(home, value + 1);
	return (expanded);
}

// Processes a word token:- handles tilde
//Skips expansion for echo args, else expands variables and removes quotes
char	*handle_word_token(t_token *token, t_program *minishell)
{
	if (minishell->cmd_list && minishell->cmd_list->args
		&& ft_strcmp(minishell->cmd_list->args[0], "echo") == 0)
		return (ft_strdup(token->value));
	if (token->value[0] == '~' && (token->value[1] == '\0'
			|| token->value[1] == '/'))
		return (handle_tilde_expansion(token->value, minishell));
	return (expand_and_remove_quotes(token->value, minishell));
}
