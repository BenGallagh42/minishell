/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_errors_handling.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:37:42 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/29 22:13:36 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Shows error for unexpected tokens
void	print_syntax_error(const char *token_value, t_program *minishell)
{
	minishell->error_code = 2;
	if (!token_value)
		token_value = "newline";
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd((char *)token_value, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

// Shows error for pipe issues
void	print_pipe_error(t_program *minishell)
{
	minishell->error_code = 2;
	ft_putendl_fd("minishell: syntax error near unexpected token `|'",
		STDERR_FILENO);
}

// Shows file errors (missing or no permission)
void	print_file_error(int error_code, const char *token_value,
		t_program *minishell)
{
	struct stat	sb;

	minishell->error_code = error_code;
	if (error_code == ERR_FILE_NOT_FOUND)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)token_value, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else if (error_code == ERR_PERMISSION_DENIED)
	{
		if (stat(token_value, &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((char *)token_value, STDERR_FILENO);
			ft_putendl_fd(": Is a directory", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((char *)token_value, STDERR_FILENO);
			ft_putendl_fd(": Permission denied", STDERR_FILENO);
		}
	}
}

// Shows general errors (command, memory, interrupt)
void	print_general_error(int error_code, const char *token_value,
		t_program *minishell)
{
	minishell->error_code = error_code;
	if (error_code == ERR_NO_COMMAND)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (token_value)
			ft_putstr_fd((char *)token_value, STDERR_FILENO);
		else
			ft_putstr_fd("", STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	else if (error_code == ERR_MEMORY)
		ft_putendl_fd("minishell: memory allocation failed", STDERR_FILENO);
	else if (error_code == ERR_INTERRUPTED)
		ft_putendl_fd("minishell: operation interrupted", STDERR_FILENO);
	else
		ft_putendl_fd("minishell: unknown error", STDERR_FILENO);
}

// Picks the right error message to show
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
		print_general_error(error_code, token_value, minishell);
}
