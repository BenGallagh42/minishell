/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_errors_handling.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:18:44 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/03 20:07:52 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(int error_code, const char *token_value,
	t_program *minishell)
{
	minishell->error_code = error_code;
	if (error_code == ERR_NO_COMMAND)
		printf("minishell: no command provided\n");
	else if (error_code == ERR_SYNTAX_TOKEN)
		printf("minishell: syntax error near unexpected token '%s'\n",
			token_value);
	else if (error_code == ERR_SYNTAX_PIPE)
		printf("minishell: syntax error: missing command before pipe\n");
	else if (error_code == ERR_FILE_NOT_FOUND)
		printf("minishell: %s: No such file or directory\n", token_value);
	else if (error_code == ERR_PERMISSION_DENIED)
		printf("minishell: %s: Permission denied\n", token_value);
	else if (error_code == ERR_MEMORY)
		printf("minishell: memory allocation failed\n");
	else if (error_code == ERR_INTERRUPTED)
		printf("minishell: operation interrupted\n");
	else
		printf("minishell: unknown error\n");
}
