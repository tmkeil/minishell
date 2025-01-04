/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/04 20:18:39 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_execute(t_minishell **minishell, char *cmd, t_lexems *lexem)
// {
// 	char	**args;
// 	size_t	size;

// 	size = ft_size(lexem);
// 	args = malloc(sizeof(char *) * (size + 1));
// 	if (!args)
// 		exit(EXIT_FAILURE);
// 	while (size + 1)
// 		args[size--] = NULL;
// 	if (!ft_handle_lexem(&args, lexem, cmd))
// 	{
// 		ft_free_ptr(&args);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (execve(cmd, args, (*minishell)->envps) == -1)
// 		ft_put_error_str((char *)lexem->value, ": command not found");
// 	ft_free_ptr(&args);
// 	exit(EXIT_FAILURE);
// }

// int	ft_builtin(t_minishell **m_shell, t_lexems *lexes, t_envs **envs,
// int ipc)
// {
// 	char	*cmd;

// 	if (lexes->type == SEPERATOR)
// 		lexes = lexes->next;
// 	cmd = ft_is_builtin(lexes->value, (*m_shell)->envps);
// 	if (!cmd)
// 		return (0);
// 	if (!ft_strncmp(cmd, "cd", 3))
// 		ft_changedir(m_shell, lexes, ipc);
// 	if (!ft_strncmp(cmd, "echo", 5))
// 		ft_echo(lexes, NULL, false);
// 	if (!ft_strncmp(cmd, "env", 4))
// 		ft_env(*envs);
// 	if (!ft_strncmp(cmd, "exit", 5))
// 		ft_exit(m_shell, lexes);
// 	if (!ft_strncmp(cmd, "export", 7))
// 		ft_export(m_shell, lexes, envs, &(*m_shell)->envps, ipc);
// 	if (!ft_strncmp(cmd, "pwd", 4))
// 		ft_pwd();
// 	if (!ft_strncmp(cmd, "unset", 6))
// 		ft_unset(m_shell, lexes, envs, &(*m_shell)->envps, ipc);
// 	return (free(cmd), 0);
// }

// void	ft_exe(t_minishell **minishell, t_lexems *lexem, t_envs **envs,
// 		int ipc)
// {
// 	char	*cmd;

// 	if (lexem->type == SEPERATOR)
// 		lexem = lexem->next;
// 	if (!lexem)
// 		exit(EXIT_FAILURE);
// 	// ft_redirections(lexem);
// 	// ft_heredoc(lexem);
// 	if (!ft_builtin(minishell, lexem, envs, ipc))
// 	{
// 		cmd = ft_getpath(lexem->value, (*minishell)->envps, true);
// 		if (!cmd)
// 			exit(INVALID_CMD);
// 		ft_execute(minishell, cmd, lexem);
// 	}
// }

// void	ft_close_write(int wr)
// {
// 	if (wr != -1)
// 		close(wr);
// }

// void	ft_close_read(int re)
// {
// 	if (re != -1)
// 		close(re);
// }

// void	ft_parent(t_minishell **minishell, int *read, int *fd, pid_t pid)
// {
// 	int	status;

// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 		(*minishell)->exit_status = WEXITSTATUS(status);
// 	else
// 		(*minishell)->exit_status = INVALID_CMD;
// 	ft_close_read(*read);
// 	ft_close_write(fd[1]);
// 	*read = fd[0];
// }

// int	ft_execute_commands(t_minishell **minishell)
// {
// 	int		i;
// 	int		new_read;
// 	pid_t	pid;

// 	i = 0;
// 	if (pipe((*minishell)->ipc) == -1)
// 		perror("pipe error");
//     // ft_send_to_ipc(minishell, (*minishell)->envps, (*minishell)->ipc[1]);
// 	new_read = -1;
// 	while ((*minishell)->table[i])
// 	{
// 		if (i < (*minishell)->nbr_pipes && pipe((*minishell)->pipe) == -1)
// 			perror("pipe error");
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			close((*minishell)->ipc[0]);
// 			ft_set_execution_sig();
// 			ft_child(minishell, &new_read, i, (*minishell)->ipc[1]);
// 		}
// 		else
// 		{
// 			close((*minishell)->ipc[1]);
// 			ft_parent(minishell, &new_read, (*minishell)->pipe, pid);
// 			ft_transfer_child_parent(minishell, &(*minishell)->envs,
// (*minishell)->ipc[0], &(*minishell)->envps);
// 			ft_init_sig();
// 		}
// 		i++;
// 	}
// 	return (close((*minishell)->ipc[0]), 1);
// }

char	*ft_is_builtin(void *value, char **envp)
{
	int		i;
	char	*path;
	char	**cmd_path;

	if (ft_strnstr(BUILTINS, (char *)value, ft_strlen(BUILTINS)))
		return (ft_strdup((char *)value));
	path = ft_getpath((char *)value, envp, false);
	if (!path)
		return (NULL);
	cmd_path = ft_split(path, '/');
	free(path);
	if (!cmd_path)
		return (NULL);
	i = 0;
	while (cmd_path[i])
		i++;
	if (ft_strnstr(BUILTINS, cmd_path[i - 1], ft_strlen(BUILTINS)))
	{
		path = ft_strdup(cmd_path[i - 1]);
		return (ft_free_ptr(&cmd_path), path);
	}
	return (ft_free_ptr(&cmd_path), NULL);
}

void	ft_redirect_pipe(int fd_in, int *fd_pipe, bool is_next)
{
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (is_next && fd_pipe[1] != -1)
	{
		dup2(fd_pipe[1], STDOUT_FILENO);
		close(fd_pipe[1]);
		fd_pipe[1] = -1;
	}
}

void	ft_child(t_minishell *minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
{
	char	*path;

	ft_set_execution_sig();
	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next == NULL));
	ft_handle_redirections(cmd, &fd_in);
	path = ft_getpath(cmd->cmd, minishell->envps, true);
	if (!path)
		exit(INVALID_CMD);
	execve(path, cmd->args, minishell->envps);
	perror("execve");
	exit(1);
}

int	run_builtin(t_minishell *minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
{
	char	*cmd_builtin;

	cmd_builtin = ft_is_builtin(cmd->cmd, minishell->envps);
	if (!cmd_builtin)
		return (0);
	ft_handle_redirections(cmd, &fd_in);
	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next == NULL));
	if (!ft_strncmp(cmd_builtin, "cd", 3))
		ft_changedir(&minishell, cmd->args);
	if (!ft_strncmp(cmd_builtin, "echo", 5))
		ft_echo(cmd->args);
	if (!ft_strncmp(cmd_builtin, "env", 4))
		ft_env(minishell->envs);
	if (!ft_strncmp(cmd_builtin, "exit", 5))
		ft_exit(&minishell, cmd->args);
	if (!ft_strncmp(cmd_builtin, "export", 7))
		ft_export(&minishell, cmd->args, &(*minishell).envs, &minishell->envps);
	if (!ft_strncmp(cmd_builtin, "pwd", 4))
		ft_pwd();
	if (!ft_strncmp(cmd_builtin, "unset", 6))
		ft_unset(&minishell, cmd->args, &(*minishell).envs, &minishell->envps);
	dup2(minishell->in_fd, STDIN_FILENO);
	dup2(minishell->out_fd, STDOUT_FILENO);
	if (fd_pipe[0] != -1)
		close(fd_pipe[0]);
	if (fd_pipe[1] != -1)
		close(fd_pipe[1]);
	return (free(cmd_builtin), 1);
}

void	execute_external(t_minishell *minishell, t_cmds *cmd, int fd_in,
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
			minishell->exit_status = WEXITSTATUS(status);
		else
			minishell->exit_status = 1;
		if (fd_pipe[1] != -1)
			close(fd_pipe[1]);
	}
}

int	ft_execute_commands(t_minishell **minishell)
{
	int		fd_pipe[2];
	int		fd_in;
	t_cmds	*current;

	fd_in = -1;
	current = (*minishell)->cmds;
	while (current)
	{
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
		if (!run_builtin(*minishell, current, fd_in, fd_pipe))
			execute_external(*minishell, current, fd_in, fd_pipe);
		if (fd_in != -1)
			close(fd_in);
		fd_in = fd_pipe[0];
		current = current->next;
	}
	return (1);
}
