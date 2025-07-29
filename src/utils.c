/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:07:34 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 19:07:36 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Checks if a string is all numbers
int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

// Prints the list of tokens
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 1;
	if (!current)
	{
		printf("No tokens generated.\n");
		return ;
	}
	printf("Lexer Output:\n");
	while (current)
	{
		printf("  Token %d: %s (Type: %d)\n", i++,
			current->value, current->type);
		current = current->next;
	}
}

// Prints redirection details
void	print_redirections(t_redirection *redirs)
{
	t_redirection	*current;
	char			*target_str;
	char			*content_str;

	current = redirs;
	while (current)
	{
		if (current->target)
			target_str = current->target;
		else
			target_str = "NULL";
		if (current->content)
			content_str = current->content;
		else
			content_str = "NULL";
		printf("    Redirection: Type %d, Target: %s, Content: %s\n",
			current->type, target_str, content_str);
		current = current->next;
	}
}

// Prints one command’s details
static void	print_single_command(t_command *cmd, int cmd_num)
{
	int	i;

	printf("  Command %d:\n", cmd_num);
	if (cmd->args)
	{
		printf("    Arguments: ");
		i = 0;
		while (cmd->args[i])
		{
			printf("%s ", cmd->args[i]);
			i++;
		}
		printf("\n");
	}
	if (cmd->redirs)
	{
		printf("    Redirections:\n");
		print_redirections(cmd->redirs);
	}
	printf("    Is Piped: %d\n", cmd->is_piped);
	printf("    Is Background: %d\n", cmd->is_background);
	printf("    Is Builtin: %d\n", cmd->is_builtin);
}

// Prints the list of commands
void	print_commands(t_command *cmd_list)
{
	t_command	*current;
	int			cmd_num;

	current = cmd_list;
	cmd_num = 1;
	if (!current)
	{
		printf("No commands parsed.\n");
		return ;
	}
	printf("Parser Output:\n");
	while (current)
	{
		print_single_command(current, cmd_num++);
		current = current->next;
	}
}
