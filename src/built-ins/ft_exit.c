#include "minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
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

static void	cleanup_and_exit(t_program *minishell, int code)
{
	free_shell(minishell);
	exit(code);
}

int	ft_exit(t_command *cmd, t_program *minishell)
{
	int	code;

	if (!cmd->args[1])
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		cleanup_and_exit(minishell, minishell->error_code);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		cleanup_and_exit(minishell, 255);
	}
	if (cmd->args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		minishell->error_code = 1;
		return (1);
	}
	code = ft_atoi(cmd->args[1]);
	ft_putendl_fd("exit", STDERR_FILENO);
	cleanup_and_exit(minishell, (unsigned char)code);
	return (0);
}
