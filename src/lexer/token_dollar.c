/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:35:22 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/25 12:45:16 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:35:22 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/22 17:30:00 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_dollar_quotes(const char **input,
	t_token **head, t_token **curr)
{
	const char	*start;
	char		*token;

	start = ++(*input);
	while (**input && **input != '"')
		(*input)++;
	if (**input == '"')
		(*input)++;
	token = ft_strndup(start - 2, *input - start + 2);
	if (!token)
		return ;
	add_token(head, curr, TKN_WORD, token);
}

static void handle_dollar_question(const char **input, t_token **head, t_token **curr, t_program *minishell)
{
    char *exit_status;

    (*input)++;
    exit_status = ft_itoa(minishell->error_code);
    if (!exit_status)
    {
        minishell->error_code = ERR_MEMORY;
        return;
    }
    add_token(head, curr, TKN_WORD, exit_status);
}

static void	handle_invalid_var(const char **input,
	t_token **head, t_token **curr)
{
	(*input)++;
	add_token(head, curr, TKN_WORD, ft_strdup(""));
}

void token_dollar(const char **input, t_token **head, t_token **curr, t_program *minishell)
{
    const char *start;
    char *var_name;
    char *var_value;

    start = *input;
    (*input)++;
    if (**input == '"')
        return (handle_dollar_quotes(input, head, curr));
    if (**input == '?')
        return (handle_dollar_question(input, head, curr, minishell));
    if (!ft_isalpha(**input) && **input != '_')
        return (handle_invalid_var(input, head, curr));
    while (**input && (ft_isalnum(**input) || **input == '_'))
        (*input)++;
    var_name = ft_strndup(start + 1, *input - start - 1);
    if (!var_name)
    {
        minishell->error_code = ERR_MEMORY;
        return;
    }
    var_value = ft_getenv(var_name, minishell->envp);
    add_token(head, curr, TKN_WORD, var_value ? ft_strdup(var_value) : ft_strdup(""));
    free(var_name);
}
