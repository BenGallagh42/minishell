/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 18:25:58 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/21 14:57:29 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_program	minishell;

	(void)argc;
	(void)argv;
	minishell.token = NULL;
	minishell.input_buffer = NULL;
	minishell.cmd_list = NULL;
	minishell.envp = NULL;
	minishell.error_code = 0;
	init_shell(&minishell, envp);
	run_shell(&minishell);
	free_shell(&minishell);
	return (minishell.error_code);
}
