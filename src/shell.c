/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:44:16 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/25 12:34:27 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "Quit\n", 5);
}

void init_shell(t_program *minishell, char **envp)
{
    int i;
    int j;
    char *shlvl_value;
    int shlvl;

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

    // Handle SHLVL
    shlvl_value = ft_getenv("SHLVL", minishell->envp);
    if (shlvl_value)
        shlvl = ft_atoi(shlvl_value) + 1;
    else
        shlvl = 1; // If SHLVL is not set, start at 1
    shlvl_value = ft_itoa(shlvl);
    if (!shlvl_value)
    {
        ft_putstr_fd("minishell: memory allocation failed\n", STDERR_FILENO);
        exit(1);
    }
    ft_setenv("SHLVL", shlvl_value, minishell);
    free(shlvl_value);

    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

void	run_shell(t_program *minishell)
{
	char		*input;
	t_token		*tokens;
	t_command	*commands;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (input && *input)
			add_history(input);
		else
		{
			free(input);
			continue ;
		}
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
