#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i = 0;

	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	cleanup_and_exit(t_program *minishell, int exit_code)
{
	free_shell(minishell);
	exit(exit_code);
}

int	ft_exit(t_command *cmd, t_program *minishell)
{
	int	exit_code;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!cmd->args[1])
		cleanup_and_exit(minishell, minishell->error_code);
	if (!is_numeric(cmd->args[1]))
	{
		print_error_message(ERR_SYNTAX_TOKEN, cmd->args[1], minishell);
		cleanup_and_exit(minishell, 255);
	}
	if (cmd->args[2])
	{
		print_error_message(ERR_SYNTAX_TOKEN, "too many arguments", minishell);
		minishell->error_code = 1;
		return (1);
	}
	exit_code = ft_atoi(cmd->args[1]);
	cleanup_and_exit(minishell, (unsigned char)exit_code);
	return (0); // unreachable, but makes compiler happy xD
}
