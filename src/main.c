/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:58 by bboulmie          #+#    #+#             */
/*   Updated: 2025/06/12 15:45:27 by bboulmie         ###   ########.fr       */
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
		printf("  Token %d: %s (Type: %d)\n", i++, current->value, current->type);
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

int	main(void)
{
	t_program	minishell;
	char		*input;
	t_token		*tokens;
	t_command	*cmd_list;

	minishell.envp = NULL;
	minishell.error_code = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		tokens = main_lexer(input, &minishell);
		if (tokens)
		{
			print_tokens(tokens);
			cmd_list = main_parser(tokens, &minishell);
			if (cmd_list)
			{
				print_commands(cmd_list);
				free_command(cmd_list);
			}
			printf("Error code: %d\n", minishell.error_code);
			free_tokens(tokens);
		}
		free(input);
	}
	return (minishell.error_code);
}
