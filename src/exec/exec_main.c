/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bboulmie <bboulmie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:08:10 by bboulmie          #+#    #+#             */
/*   Updated: 2025/07/25 12:45:52 by bboulmie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	generate_tmp_filename(char *buffer, int count)
{
	char	*base = "/tmp/minishell_heredoc_";
	char	*num;
	int		i;

	i = 0;
	while (base[i])
	{
		buffer[i] = base[i];
		i++;
	}
	num = ft_itoa(count);
	if (!num)
		return ;
	while (*num)
	{
		buffer[i] = *num++;
		i++;
	}
	buffer[i] = '\0';
}

static void	close_pipe_ends(int *prev_pipe, int *pipefd, int is_piped)
{
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (is_piped)
	{
		close(pipefd[1]);
		*prev_pipe = pipefd[0];
	}
	else
		*prev_pipe = -1;
}

static int	apply_redirections(t_redirection *redirs, t_program *mini)
{
	int	err_code;

	while (redirs)
	{
		if (redirs->type == TKN_REDIR_OUT || redirs->type == TKN_REDIR_APPEND)
		{
			int fd = open(redirs->target,
				redirs->type == TKN_REDIR_OUT ? O_WRONLY | O_CREAT | O_TRUNC
				: O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				err_code = (errno == EACCES || errno == EISDIR) ? ERR_PERMISSION_DENIED : ERR_FILE_NOT_FOUND;
				return (print_error_message(err_code, redirs->target, mini), 0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirs->type == TKN_REDIR_IN)
		{
			if (!redirs->target || !*redirs->target)
				return (print_error_message(ERR_FILE_NOT_FOUND, NULL, mini), 0);
			int fd = open(redirs->target, O_RDONLY);
			if (fd == -1)
			{
				err_code = (errno == EACCES) ? ERR_PERMISSION_DENIED : ERR_FILE_NOT_FOUND;
				return (print_error_message(err_code, redirs->target, mini), 0);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redirs->type == TKN_REDIR_HEREDOC)
		{
			char tmp_file[32];
			static int heredoc_count = 0;
			generate_tmp_filename(tmp_file, heredoc_count++);
			int fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (print_error_message(ERR_FILE_NOT_FOUND, "heredoc", mini), 0);
			write(fd, redirs->content, ft_strlen(redirs->content));
			close(fd);
			fd = open(tmp_file, O_RDONLY);
			if (fd == -1)
			{
				unlink(tmp_file);
				return (print_error_message(ERR_FILE_NOT_FOUND, "heredoc", mini), 0);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
			unlink(tmp_file);
		}
		redirs = redirs->next;
	}
	return (1);
}

static void	exec_child(t_command *cmd, t_program *mini, int prev_pipe, int *pipefd)
{
	char	*cmd_path;

	// Set default signal handling for child
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (cmd->is_piped)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (!apply_redirections(cmd->redirs, mini))
	{
		free_command(cmd);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
	{
		mini->error_code = ERR_NO_COMMAND;
		print_error_message(mini->error_code, cmd->args ? cmd->args[0] : "", mini);
		free_command(cmd);
		exit(127);
	}
	cmd_path = find_command_path(cmd->args[0], mini);
	if (!cmd_path)
	{
		print_error_message(mini->error_code, cmd->args[0], mini);
		free_command(cmd);
		if (mini->error_code == ERR_PERMISSION_DENIED)
			exit(126);
		exit(127);
	}
	execve(cmd_path, cmd->args, mini->envp);
	mini->error_code = ERR_NO_COMMAND;
	print_error_message(mini->error_code, cmd->args[0], mini);
	free(cmd_path);
	free_command(cmd);
	exit(127);
}

static void exec_loop(t_command *cmd, t_program *mini, pid_t *pids, int count)
{
    int i;
    int prev_pipe;
    int pipefd[2];
    int stdout_backup;

    i = -1;
    prev_pipe = -1;
    while (++i < count)
    {
        if (cmd->is_builtin && count == 1)
        {
            stdout_backup = dup(STDOUT_FILENO);
            if (stdout_backup == -1)
            {
                mini->error_code = ERR_MEMORY;
                print_error_message(ERR_MEMORY, NULL, mini);
                return;
            }
            if (!apply_redirections(cmd->redirs, mini))
            {
                dup2(stdout_backup, STDOUT_FILENO);
                close(stdout_backup);
                return;
            }
            execute_builtin(cmd, mini);
            dup2(stdout_backup, STDOUT_FILENO);
            close(stdout_backup);
            return;
        }
        if (cmd->is_piped && pipe(pipefd) == -1)
        {
            mini->error_code = ERR_PIPE;
            print_error_message(ERR_PIPE, NULL, mini);
            return;
        }
        pids[i] = fork();
        if (pids[i] == -1)
        {
            mini->error_code = ERR_FORK;
            print_error_message(ERR_FORK, NULL, mini);
            return;
        }
        if (pids[i] == 0)
            exec_child(cmd, mini, prev_pipe, pipefd);
        close_pipe_ends(&prev_pipe, pipefd, cmd->is_piped);
        cmd = cmd->next;
    }
    if (prev_pipe != -1)
        close(prev_pipe);
}

void execute_commands(t_command *cmd, t_program *minishell)
{
    t_command *tmp;
    int count;
    pid_t *pids;
    int status;
    int j;

    count = 0;
    tmp = cmd;
    while (tmp && ++count)
        tmp = tmp->next;
    if (count == 1 && cmd->is_builtin)
    {
        exec_loop(cmd, minishell, NULL, count);
        return;
    }
    pids = malloc(sizeof(pid_t) * count);
    if (!pids)
    {
        print_error_message(ERR_MEMORY, NULL, minishell);
        return;
    }
    ft_memset(pids, 0, sizeof(pid_t) * count);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    exec_loop(cmd, minishell, pids, count);
    j = -1;
    while (++j < count)
    {
        if (pids[j] > 0)
        {
            waitpid(pids[j], &status, 0);
            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
                write(STDOUT_FILENO, "Quit\n", 5);
            if (WIFEXITED(status))
                minishell->error_code = WEXITSTATUS(status);
        }
    }
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    free(pids);
}
