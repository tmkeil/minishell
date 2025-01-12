/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/12 02:01:19 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_close_fd(int *fd)
// {
// 	if (*fd != -1)
// 	{
// 		close(*fd);
// 		*fd = -1;
// 	}
// }

// int	ft_run_builtin(t_minishell **minishell, t_cmds **cmd, int *fd_in,
// 		int *fd_pipe)
// {
// 	int		status;
// 	char	*cmd_builtin;

// 	(void)fd_in;
// 	(void)fd_pipe;
// 	status = -1;
// 	cmd_builtin = ft_is_builtin((*cmd)->cmd);
// 	if (!cmd_builtin)
// 		return (-1);
// 	status = ft_choose_builtin(minishell, cmd_builtin, (*cmd)->args);
// 	free(cmd_builtin);
// 	(*minishell)->exit_status = status;
// 	return (status);
// }

// void	ft_child(t_minishell **minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
// {
// 	char	*path;

// 	ft_set_execution_sig();
// 	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
// 	ft_handle_redirections(cmd, &fd_in);
// 	if (fd_in != -1)
// 	{
// 		dup2(fd_in, STDIN_FILENO);
// 		close(fd_in);
// 		fd_in = -1;
// 	}
// 	if (cmd->next)
// 	{
// 		if (fd_pipe[0] != -1)
// 			close(fd_pipe[0]);
// 	}
// 	else
// 	{
// 		if (fd_pipe[1] != -1)
// 			close(fd_pipe[1]);
// 	}
// 	if (ft_run_builtin(minishell, &cmd, &fd_in, fd_pipe) >= 0)
// 		exit((*minishell)->exit_status);
// 	path = ft_getpath(cmd->cmd, (*minishell)->envps, true);
// 	if (!path)
// 	{
// 		ft_putstr_fd("bash: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		exit(INVALID_CMD);
// 	}
// 	execve(path, cmd->args, (*minishell)->envps);
// 	perror("execve");
// 	exit(EXIT_FAILURE);
// }

// void	ft_execute_external(t_minishell **minishell, t_cmds *cmd, int fd_in,
// 		int *fd_pipe)
// {
// 	int		status;
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == 0)
// 		ft_child(minishell, cmd, fd_in, fd_pipe);
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			(*minishell)->exit_status = WEXITSTATUS(status);
// 		if (fd_pipe[1] != -1)
// 		{
// 			close(fd_pipe[1]);
// 			fd_pipe[1] = -1;
// 		}
// 	}
// }

// int	ft_execute_commands(t_minishell **minishell)
// {
// 	int		fd_pipe[2];
// 	int		fd_in;
// 	t_cmds	*current;

// 	fd_in = -1;
// 	current = (*minishell)->cmds;
// 	if (!current->next)
// 	{
// 		fd_pipe[0] = -1;
// 		fd_pipe[1] = -1;
// 		if (ft_is_builtin(current->cmd))
// 		{
// 			ft_handle_redirections(current, &fd_in);
// 			ft_run_builtin(minishell, &current, &fd_in, fd_pipe);
// 			if (fd_in != -1)
// 				close(fd_in);
// 		}
// 		else
// 		{
// 			ft_execute_external(minishell, current, fd_in, fd_pipe);
// 			if (fd_in != -1)
// 			{
// 				close(fd_in);
// 				fd_in = -1;
// 			}
// 		}
// 		return (1);
// 	}
// 	while (current)
// 	{
// 		if (current->next)
// 			pipe(fd_pipe);
// 		else
// 		{
// 			fd_pipe[0] = -1;
// 			fd_pipe[1] = -1;
// 		}
// 		if (ft_is_builtin(current->cmd))
// 		{
// 			ft_redirect_pipe(fd_in, fd_pipe, (current->next != NULL));
// 			ft_handle_redirections(current, &fd_in);
// 			ft_run_builtin(minishell, &current, &fd_in, fd_pipe);
// 			if (fd_pipe[1] != -1)
// 			{
// 				close(fd_pipe[1]);
// 				fd_pipe[1] = -1;
// 			}
// 			ft_close_fd(&fd_in);
// 			fd_in = fd_pipe[0];
// 			dup2((*minishell)->in_fd, STDIN_FILENO);
// 			dup2((*minishell)->out_fd, STDOUT_FILENO);
// 		}
// 		else
// 		{
// 			ft_execute_external(minishell, current, fd_in, fd_pipe);
// 			if (fd_in != -1)
// 			{
// 				close(fd_in);
// 				fd_in = -1;
// 			}
// 			fd_in = fd_pipe[0];
// 		}
// 		current = current->next;
// 	}
// 	return (1);
// }

void	ft_close_fd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

int	ft_run_builtin(t_minishell **minishell, t_cmds **cmd, int *fd_in,
		int *fd_pipe)
{
	int	status;

	if (!ft_strnstr(BUILTINS, (*cmd)->cmd, ft_strlen(BUILTINS)))
		return (-1);
	ft_redirect_pipe(*fd_in, fd_pipe, ((*cmd)->next != NULL));
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

void	ft_child(t_minishell **minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
{
	char	*path;

	ft_set_execution_sig();
	if (cmd->next)
		ft_close_fd(&fd_pipe[0]);
	else
		ft_close_fd(&fd_pipe[1]);
	// if (!cmd->cmd && cmd->heredoc)
	// {
	// 	ft_close_fd(&fd_pipe[0]);
	// 	ft_close_fd(&fd_pipe[1]);
	// 	ft_close_fd(&fd_in);
	// 	term_fd = open("/dev/tty", O_RDONLY);
	// 	if (term_fd != -1)
	// 	{
	// 		dup2(term_fd, STDIN_FILENO);
	// 		close(term_fd);
	// 	}
	// 	heredoc_fd = ft_setup_heredoc(cmd);
	// 	if (heredoc_fd != -1)
	// 	{
	// 		dup2(heredoc_fd, STDIN_FILENO);
	// 		close(heredoc_fd);
	// 	}
	// 	exit(0);
	// }
	if (ft_run_builtin(minishell, &cmd, &fd_in, fd_pipe) >= 0)
		exit((*minishell)->exit_status);
	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
	ft_handle_redirections(cmd, &fd_in);
	if (!cmd->cmd)
		exit(0);
	path = ft_getpath(cmd->cmd, (*minishell)->envps, true);
	if (!path)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(INVALID_CMD);
	}
	execve(path, cmd->args, (*minishell)->envps);
	exit(EXIT_FAILURE);
}

void	ft_execute_external(t_minishell **minishell, t_cmds *cmd, int *fd_in,
		int *fd_pipe)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		ft_child(minishell, cmd, *fd_in, fd_pipe);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*minishell)->exit_status = WEXITSTATUS(status);
		ft_close_fd(&fd_pipe[1]);
		ft_close_fd(fd_in);
	}
}

void	ft_execute_command_in_pipeline(t_minishell **minishell,
		t_cmds **current, int *fd_in)
{
	int	fd_pipe[2];

	if ((*current)->next)
	{
		if (pipe(fd_pipe) == -1)
			return ;
	}
	else
	{
		fd_pipe[0] = -1;
		fd_pipe[1] = -1;
	}
	if (ft_run_builtin(minishell, current, fd_in, fd_pipe) != -1)
	{
		ft_close_fd(&fd_pipe[1]);
		ft_close_fd(fd_in);
		*fd_in = fd_pipe[0];
		dup2((*minishell)->in_fd, STDIN_FILENO);
		dup2((*minishell)->out_fd, STDOUT_FILENO);
	}
	else
	{
		ft_execute_external(minishell, *current, fd_in, fd_pipe);
		*fd_in = fd_pipe[0];
	}
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
		ft_execute_external(minishell, cmds, &fd_in, fd_pipe);
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
