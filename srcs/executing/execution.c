/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 18:00:19 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_child(t_minishell **minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
{
	char	*path;

	ft_set_execution_sig();
	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
	ft_handle_redirections(cmd, &fd_in);
	if (ft_run_builtin(minishell, &cmd, fd_in, fd_pipe) >= 0)
		exit((*minishell)->exit_status);
	path = ft_getpath(cmd->cmd, (*minishell)->envps, true);
	if (!path)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(INVALID_CMD);
	}
	execve(path, cmd->args, (*minishell)->envps);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	ft_execute_external(t_minishell **minishell, t_cmds *cmd, int fd_in,
		int *fd_pipe)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		ft_child(minishell, cmd, fd_in, fd_pipe);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*minishell)->exit_status = WEXITSTATUS(status);
		if (fd_pipe[1] != -1)
			close(fd_pipe[1]);
	}
}

int	ft_run_builtin(t_minishell **minishell, t_cmds **cmd, int fd_in, int *fd_pipe)
{
	int		status;
	char	*cmd_builtin;

	status = -1;
	cmd_builtin = ft_is_builtin((*cmd)->cmd, (*minishell)->envps);
	if (!cmd_builtin)
		return (-1);
	ft_redirect_pipe(fd_in, fd_pipe, ((*cmd)->next != NULL));
	ft_handle_redirections(*cmd, &fd_in);
	status = ft_choose_builtin(minishell, cmd_builtin, (*cmd)->args);
	dup2((*minishell)->in_fd, STDIN_FILENO);
	dup2((*minishell)->out_fd, STDOUT_FILENO);
	if (fd_pipe[1] != -1)
		close(fd_pipe[1]);
	free(cmd_builtin);
	(*minishell)->exit_status = status;
	return (status);
}

int	ft_execute_commands(t_minishell **minishell)
{
	int		fd_pipe[2];
	int		fd_in;
	t_cmds	*current;

	fd_in = -1;
	current = (*minishell)->cmds;
	if (!current->next)
	{
		fd_pipe[0] = -1;
		fd_pipe[1] = -1;
		// printf("e1\n");
		if (ft_run_builtin(minishell, &current, fd_in, fd_pipe) != -1)
			return (1);
		// printf("e2\n");
	}
	while (current)
	{
		printf("test ksbdvisdiuvdsvbisudvbdsviubdsiubvsibdvisdbvisbdv\n");
		if (current->next)
		{
			if (pipe(fd_pipe) == -1)
				return (perror("pipe error"), 1);
		}
		else
		{
			fd_pipe[0] = -1;
			fd_pipe[1] = -1;
		}
		ft_execute_external(minishell, current, fd_in, fd_pipe);
		if (fd_in != -1)
			close(fd_in);
		fd_in = fd_pipe[0];
		current = current->next;
	}
	return (1);
}
