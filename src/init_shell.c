/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnithyan <hnithyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:22:46 by hnithyan          #+#    #+#             */
/*   Updated: 2025/07/29 14:23:04 by hnithyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_envp(t_program *minishell, char **envp)
{
	int		i;
	int		j;

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
}

static void	setup_shlvl(t_program *minishell)
{
	char	*shlvl_value;
	int		shlvl;

	shlvl_value = ft_getenv("SHLVL", minishell->envp);
	if (shlvl_value)
		shlvl = ft_atoi(shlvl_value) + 1;
	else
		shlvl = 1;
	shlvl_value = ft_itoa(shlvl);
	if (!shlvl_value)
	{
		ft_putstr_fd("minishell: memory allocation failed\n", STDERR_FILENO);
		exit(1);
	}
	ft_setenv("SHLVL", shlvl_value, minishell);
	free(shlvl_value);
}

static void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_shell(t_program *minishell, char **envp)
{
	copy_envp(minishell, envp);
	minishell->error_code = 0;
	setup_shlvl(minishell);
	setup_signals();
}
