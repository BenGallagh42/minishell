/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_errors_handling.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:18:44 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/18 17:39:57 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints a syntax error for an unexpected token like newline or '|'
void	print_syntax_error(const char *token_value, t_program *minishell)
{
	minishell->error_code = 2;
	if (!token_value)
		token_value = "newline";
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd((char *)token_value, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

// Prints a syntax error specifically for a pipe token
void	print_pipe_error(t_program *minishell)
{
	minishell->error_code = 2;
	ft_putendl_fd("minishell: syntax error near unexpected token `|'", STDERR_FILENO);
}

// Prints a file related error like file not found or permission denied
void	print_file_error(int error_code, const char *token_value,
	t_program *minishell)
{
	minishell->error_code = error_code;
	if (error_code == ERR_FILE_NOT_FOUND)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)token_value, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else if (error_code == ERR_PERMISSION_DENIED)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)token_value, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	}
}

// Prints general errors like memory failure or no command given
void	print_general_error(int error_code, t_program *minishell)
{
	minishell->error_code = error_code;
	if (error_code == ERR_NO_COMMAND)
		ft_putendl_fd("minishell: no command provided", STDERR_FILENO);
	else if (error_code == ERR_MEMORY)
		ft_putendl_fd("minishell: memory allocation failed", STDERR_FILENO);
	else if (error_code == ERR_INTERRUPTED)
		ft_putendl_fd("minishell: operation interrupted", STDERR_FILENO);
	else
		ft_putendl_fd("minishell: unknown error", STDERR_FILENO);
}

// Dispatches error printing based on the error code type
void	print_error_message(int error_code, const char *token_value,
	t_program *minishell)
{
	if (error_code == ERR_SYNTAX_TOKEN)
		print_syntax_error(token_value, minishell);
	else if (error_code == ERR_SYNTAX_PIPE)
		print_pipe_error(minishell);
	else if (error_code == ERR_FILE_NOT_FOUND
		|| error_code == ERR_PERMISSION_DENIED)
		print_file_error(error_code, token_value, minishell);
	else
		print_general_error(error_code, minishell);
}
