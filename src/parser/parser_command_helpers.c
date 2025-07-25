/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:00:01 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/25 20:07:37 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the command requires file argument validation
int	needs_file_validation(const char *cmd)
{
	const char	*file_cmds[] = {"head", "tail", NULL};
	int			i;

	i = 0;
	while (file_cmds[i])
	{
		if (ft_strcmp(cmd, file_cmds[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

// Validates file existence and readability for command arguments
int	validate_file_arg(const char *arg, t_program *minishell)
{
	if (access(arg, F_OK | R_OK) != 0)
	{
		print_error_message(ERR_FILE_NOT_FOUND, arg, minishell);
		return (0);
	}
	return (1);
}

// Expands a single token and returns a list containing the expanded result
t_list	*get_expanded_list(t_token *token, t_program *minishell)
{
	char	*expanded;

	if (token->type == TKN_WILDCARD)
		return (expand_wildcard(token->value, minishell));
	if (token->type == TKN_STATUS)
	{
		expanded = ft_itoa(minishell->error_code);
		if (!expanded)
		{
			print_error_message(ERR_MEMORY, NULL, minishell);
			return (NULL);
		}
		return (ft_lstnew(expanded));
	}
	if (token->type == TKN_WORD)
	{
		expanded = handle_word_token(token, minishell);
		if (expanded)
			return (ft_lstnew(expanded));
	}
	return (NULL);
}

// Appends the contents of to_add to args and clears to_add
void	append_expanded(t_list **args, t_list *to_add)
{
	if (!to_add)
		return ;
	if (!*args)
		*args = to_add;
	else
		ft_lstlast(*args)->next = to_add;
}

// Set all bytes of the t_command structure to zero/NULL
void	init_command(t_command *cmd)
{
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->is_piped = 0;
	cmd->is_builtin = 0;
	cmd->is_background = 0;
}
