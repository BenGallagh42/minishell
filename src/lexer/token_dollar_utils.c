/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_dollar_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 03:32:05 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/27 03:32:18 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*safe_strdup_env(char *var_value)
{
	if (var_value)
		return (ft_strdup(var_value));
	return (ft_strdup(""));
}

void	handle_env_var_value(t_token **head, t_token **curr,
	char *var_name, t_program *minishell)
{
	char	*var_value;

	var_value = ft_getenv(var_name, minishell->envp);
	add_token(head, curr, TKN_WORD, safe_strdup_env(var_value));
	free(var_name);
}

void	handle_invalid_var(const char **input,
	t_token **head, t_token **curr)
{
	(*input)++;
	add_token(head, curr, TKN_WORD, ft_strdup(""));
}
