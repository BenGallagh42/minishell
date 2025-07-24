/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:44:16 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/22 16:20:25 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Initializes minishell environment variables and sets default signal handlers
void	init_shell(t_program *minishell, char **envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
		i++;
	minishell->envp = malloc(sizeof(char *) * (i + 1));
	if (!minishell->envp)
	{
		ft_putstr_fd("minishell: memory allocation failed\n", STDERR_FILENO);
		exit(1);
	}
	while (j < i)
	{
		minishell->envp[j] = ft_strdup(envp[j]);
		j++;
	}
	minishell->envp[i] = NULL;
	minishell->error_code = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

// Main shell loop which reads input, lexes, parses, executes, and frees memory
void	run_shell(t_program *minishell)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (input && *input)
			add_history(input);
		tokens = main_lexer(input, minishell);
		if (tokens)
		{
			commands = main_parser(tokens, minishell);
			if (commands)
			{
				execute_commands(commands, minishell);
				free_command(commands);
			}
			free_tokens(tokens);
		}
		free(input);
	}
}

// Frees the duplicated environment variable array
void	free_shell(t_program *minishell)
{
	int	i;

	i = 0;
	while (minishell->envp[i])
	{
		free(minishell->envp[i]);
		i++;
	}
	free(minishell->envp);
}

// Frees the linked list of tokens
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
