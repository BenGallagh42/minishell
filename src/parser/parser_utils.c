/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:56:06 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/03 18:04:50 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
		i++;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

t_redirection	*list_to_redir_array(t_list *list)
{
	t_redirection	*head;
	t_redirection	*tail;
	t_list			*current;
	t_redirection	*redir;

	head = NULL;
	tail = NULL;
	current = list;
	while (current)
	{
		redir = (t_redirection *)current->content;
		if (!head)
			head = redir;
		else
			tail->next = redir;
		tail = redir;
		current = current->next;
	}
	return (head);
}

t_syntax_tree	*build_pipeline_node(t_command **cmds, int cmd_count)
{
	t_syntax_tree	*node;
	int				i;

	node = malloc(sizeof(t_syntax_tree));
	if (!node)
	{
		i = 0;
		while (i < cmd_count)
			free_command(cmds[i++]);
		free(cmds);
		return (NULL);
	}
	if (cmd_count == 1)
	{
		node->e_type = TYPE_COMMAND;
		node->u_data.command = cmds[0];
		free(cmds);
	}
	else
	{
		node->e_type = TYPE_PIPELINE;
		node->u_data.s_pipeline.commands = cmds;
		node->u_data.s_pipeline.cmd_count = cmd_count;
	}
	return (node);
}
