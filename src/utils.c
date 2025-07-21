/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:05:05 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 12:42:16 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	print_commands(t_command *cmd_list)
{
	t_command	*current;
	int			cmd_num;
	int			i;

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
		printf("  Command %d:\n", cmd_num++);
		if (current->args)
		{
			printf("    Arguments: ");
			i = 0;
			while (current->args[i])
			{
				printf("%s ", current->args[i]);
				i++;
			}
			printf("\n");
		}
		if (current->redirs)
		{
			printf("    Redirections:\n");
			print_redirections(current->redirs);
		}
		printf("    Is Piped: %d\n", current->is_piped);
		printf("    Is Background: %d\n", current->is_background);
		printf("    Is Builtin: %d\n", current->is_builtin);
		current = current->next;
	}
}
