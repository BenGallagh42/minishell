/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:36:13 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 18:36:15 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a command and its linked commands
void	free_command(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->args)
			ft_free_array(cmd->args);
		if (cmd->redirs)
			free_redirs(cmd->redirs);
		free(cmd);
		cmd = next;
	}
}

// Frees a list of redirections
void	free_redirs(t_redirection *redirs)
{
	t_redirection	*next;

	while (redirs)
	{
		next = redirs->next;
		free(redirs->target);
		free(redirs->content);
		free(redirs);
		redirs = next;
	}
}

// Frees one redirection node
void	free_redirection(void *content)
{
	t_redirection	*redir;

	redir = (t_redirection *)content;
	if (redir)
	{
		free(redir->target);
		free(redir->content);
		free(redir);
	}
}

// Frees a command and its argument/redirection lists
void	free_cmd(t_command *cmd, t_list *args, t_list *redirs)
{
	if (cmd)
	{
		if (cmd->args)
			ft_free_array(cmd->args);
		if (cmd->redirs)
			free_redirs(cmd->redirs);
		free(cmd);
	}
	if (args)
		ft_lstclear(&args, free);
	if (redirs)
		ft_lstclear(&redirs, free_redirection);
}

// Frees temporary argument and redirection lists
void	free_lists(t_list **args, t_list **redirs)
{
	if (args && *args)
		ft_lstclear(args, free);
	(void)redirs;
}
