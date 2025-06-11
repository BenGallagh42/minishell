/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:02:26 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/11 18:38:45 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Frees a command structure */
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

/* Frees an entire linked list of redirections */
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

/* Frees a single redirection node */
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

/* Frees a command and its lists */
void	free_cmd(t_command *cmd, t_list *args, t_list *redirs)
{
	if (cmd)
	{
		free(cmd->args);
		free(cmd->redirs);
		free(cmd);
	}
	ft_lstclear(&args, free);
	ft_lstclear(&redirs, free_redirection);
}

/* Frees temporary lists */
void	free_lists(t_list **args, t_list **redirs)
{
	if (*args)
		ft_lstclear(args, free);
	if (*redirs)
		ft_lstclear(redirs, NULL);
}
