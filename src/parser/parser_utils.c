/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:53:00 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:53:03 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if a command is built into the shell
int	is_builtin(const char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset",
		"env", "exit", NULL};
	int			i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

// Turns a string list into an array
char	**list_to_array(t_list *list)
{
	size_t	size;
	char	**array;
	size_t	i;
	t_list	*current;

	size = ft_lstsize(list);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	current = list;
	while (current)
	{
		array[i] = ft_strdup(current->content);
		if (!array[i])
			return (free_array_partial(array, i), NULL);
		i++;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

// Turns a redirection list into a linked structure
t_redirection	*list_to_redir_array(t_list *list)
{
	t_redirection	*head;
	t_redirection	*new;
	t_redirection	*tail;
	t_list			*current;

	head = NULL;
	tail = NULL;
	current = list;
	while (current)
	{
		new = malloc(sizeof(t_redirection));
		if (!new)
			return (free_redirs(head), NULL);
		*new = *(t_redirection *)current->content;
		new->target = ft_strdup(((t_redirection *)current->content)->target);
		new->content = ft_strdup(((t_redirection *)current->content)->content);
		new->next = NULL;
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		current = current->next;
	}
	return (head);
}

// Checks if a token is a redirection
int	is_redirection_token(t_token_type type)
{
	if (type == TKN_REDIR_IN || type == TKN_REDIR_OUT
		|| type == TKN_REDIR_APPEND || type == TKN_REDIR_HEREDOC)
		return (1);
	return (0);
}

// Removes quotes from a string
char	*remove_quotes(const char *str)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str);
	if (len >= 2 && (str[0] == '"' || str[0] == '\'') && str[len - 1] == str[0])
	{
		new_str = ft_substr(str, 1, len - 2);
		return (new_str);
	}
	return (ft_strdup(str));
}
