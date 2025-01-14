/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/14 13:27:40 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_run_builtin(t_minishell **minishell, t_cmds **cmd, int *fd_in,
		int *fd_pipe)
{
	int	status;

	if (!ft_strnstr(BUILTINS, (*cmd)->cmd, ft_strlen(BUILTINS))
		|| !*(*cmd)->cmd)
		return (-1);
	ft_redirect_pipe(fd_in, fd_pipe, ((*cmd)->next != NULL));
	ft_handle_redirections(*cmd, fd_in);
	if (!(*cmd)->cmd)
	{
		ft_close_fd(fd_in);
		dup2((*minishell)->in_fd, STDIN_FILENO);
		dup2((*minishell)->out_fd, STDOUT_FILENO);
		return (0);
	}
	status = ft_choose_builtin(minishell, (*cmd)->cmd, (*cmd)->args);
	(*minishell)->exit_status = status;
	ft_close_fd(fd_in);
	dup2((*minishell)->in_fd, STDIN_FILENO);
	dup2((*minishell)->out_fd, STDOUT_FILENO);
	return (status);
}

void	ft_child(t_minishell **minishell, t_cmds *cmd, int *fd_in, int *fd_pipe)
{
	char	*path;

	if (!cmd->cmd)
		exit(0);
	ft_set_execution_sig();
	if (cmd->next)
		ft_close_fd(&fd_pipe[0]);
	else
		ft_close_fd(&fd_pipe[1]);
	if (ft_run_builtin(minishell, &cmd, fd_in, fd_pipe) >= 0)
		exit((*minishell)->exit_status);
	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
	ft_handle_redirections(cmd, fd_in);
	path = ft_getpath(cmd->cmd, (*minishell)->envps, &cmd->args);
	if (!path || !*(*cmd).cmd)
	{
		ft_sterr("bash: ", cmd->cmd, ": command not found");
		exit(INVALID_CMD);
	}
	execve(path, cmd->args, (*minishell)->envps);
	exit(EXIT_FAILURE);
}

int	ft_execute_pipeline(t_minishell **minishell, t_cmds *current)
{
	int	fd_in;

	fd_in = -1;
	while (current)
	{
		ft_execute_command_in_pipeline(minishell, &current, &fd_in);
		current = current->next;
	}
	return (1);
}

int	ft_execute_single_command(t_minishell **minishell, t_cmds *cmds)
{
	int	fd_in;
	int	fd_pipe[2];

	fd_in = -1;
	fd_pipe[0] = -1;
	fd_pipe[1] = -1;
	if (ft_run_builtin(minishell, &cmds, &fd_in, fd_pipe) == -1)
		ft_execute(minishell, cmds, &fd_in, fd_pipe);
	return (1);
}

int	ft_execute_commands(t_minishell **minishell)
{
	t_cmds	*current;

	current = (*minishell)->cmds;
	if (!(*minishell)->cmds->next)
		return (ft_execute_single_command(minishell, current));
	return (ft_execute_pipeline(minishell, current));
}
