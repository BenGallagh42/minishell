/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:01:23 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/30 16:44:52 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Starts the minishell program
int	main(int argc, char **argv, char **envp)
{
	t_program	minishell;

	if (argc > 1)
	{
		ft_printf("minishell: %s: No such file or directory\n", argv[1]);
		exit (127);
	}
	else
	{
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
}
