/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:02:26 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/03 18:08:03 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_redirection(void *content)
{
	t_redirection	*redir;

	redir = (t_redirection *)content;
	if (redir)
	{
		free(redir->target);
		free(redir);
	}
}

void	free_command(t_command *cmd)
{
	int				i;
	t_redirection	*redir;
	t_redirection	*next;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	redir = cmd->redirs;
	while (redir)
	{
		next = redir->next;
		free_redirection(redir);
		redir = next;
	}
	free(cmd);
}

void	free_syntax_tree(t_syntax_tree *node)
{
	int	i;

	if (!node)
		return ;
	if (node->e_type == TYPE_COMMAND)
		free_command(node->u_data.command);
	else if (node->e_type == TYPE_PIPELINE)
	{
		i = 0;
		while (i < node->u_data.s_pipeline.cmd_count)
			free_command(node->u_data.s_pipeline.commands[i++]);
		free(node->u_data.s_pipeline.commands);
	}
	else if (node->e_type == TYPE_LOGICAL)
	{
		free_syntax_tree(node->u_data.s_logical.left);
		free_syntax_tree(node->u_data.s_logical.right);
	}
	free(node);
}
