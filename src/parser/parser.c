/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:31:38 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/03 20:23:44 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_list	*collect_commands(t_token **tokens, t_program *minishell)
{
	t_list		*cmd_list;
	t_command	*cmd;

	cmd_list = NULL;
	cmd = parse_simple_cmd(tokens, minishell);
	if (!cmd)
		return (NULL);
	ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
	while (*tokens && (*tokens)->type == TKN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd = parse_simple_cmd(tokens, minishell);
		if (!cmd)
		{
			ft_lstclear(&cmd_list, (void (*)(void *))free_command);
			return (NULL);
		}
		ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
	}
	return (cmd_list);
}

static t_command	**list_to_command_array(t_list *cmd_list, int *cmd_count)
{
	t_command	**commands;
	t_list		*current;
	int			i;

	*cmd_count = ft_lstsize(cmd_list);
	commands = malloc(sizeof(t_command *) * (*cmd_count));
	if (!commands)
		return (NULL);
	current = cmd_list;
	i = 0;
	while (current)
	{
		commands[i] = (t_command *)current->content;
		current = current->next;
		i++;
	}
	return (commands);
}

static t_syntax_tree	*parse_pipeline(t_token **tokens, t_program *minishell)
{
	t_list			*cmd_list;
	t_command		**commands;
	int				cmd_count;
	t_syntax_tree	*node;

	cmd_list = collect_commands(tokens, minishell);
	if (!cmd_list)
		return (NULL);
	commands = list_to_command_array(cmd_list, &cmd_count);
	ft_lstclear(&cmd_list, NULL);
	if (!commands)
		return (NULL);
	node = build_pipeline_node(commands, cmd_count);
	return (node);
}

static t_syntax_tree	*parse_logical(t_token **tokens, t_program *minishell)
{
	t_syntax_tree	*left;
	t_syntax_tree	*node;

	left = parse_pipeline(tokens, minishell);
	while (*tokens && ((*tokens)->type == TKN_AND || (*tokens)->type == TKN_OR))
	{
		node = malloc(sizeof(t_syntax_tree));
		if (!node)
			return (NULL);
		node->e_type = TYPE_LOGICAL;
		node->u_data.s_logical.left = left;
		if ((*tokens)->type == TKN_AND)
			node->u_data.s_logical.operator = LOGICAL_AND;
		else
			node->u_data.s_logical.operator = LOGICAL_OR;
		*tokens = (*tokens)->next;
		node->u_data.s_logical.right = parse_pipeline(tokens, minishell);
		left = node;
	}
	return (left);
}

t_syntax_tree	*parse_tokens(t_token *tokens, t_program *minishell)
{
	t_syntax_tree	*root;

	if (!tokens)
		return (NULL);
	root = parse_logical(&tokens, minishell);
	if (tokens && tokens->type != TKN_EOF)
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			tokens->value);
		minishell->error_code = 2;
		free_syntax_tree(root);
		return (NULL);
	}
	return (root);
}
