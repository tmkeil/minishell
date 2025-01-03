/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 17:09:25 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute(t_minishell **minishell, char *cmd, t_lexems *lexem)
{
	char	**args;
	size_t	size;

	size = ft_size(lexem);
	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		exit(EXIT_FAILURE);
	while (size + 1)
		args[size--] = NULL;
	if (!ft_handle_lexem(&args, lexem, cmd))
	{
		ft_free_ptr(&args);
		exit(EXIT_FAILURE);
	}
	if (execve(cmd, args, (*minishell)->envps) == -1)
		ft_put_error_str((char *)lexem->value, ": command not found");
	ft_free_ptr(&args);
	exit(EXIT_FAILURE);
}

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

int	ft_builtin(t_minishell **m_shell, t_lexems *lexes, t_envs **envs, int ipc)
{
	char	*cmd;

	if (lexes->type == SEPERATOR)
		lexes = lexes->next;
	cmd = ft_is_builtin(lexes->value, (*m_shell)->envps);
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "cd", 3))
		ft_changedir(m_shell, lexes, ipc);
	if (!ft_strncmp(cmd, "echo", 5))
		ft_echo(lexes, NULL, false);
	if (!ft_strncmp(cmd, "env", 4))
		ft_env(*envs);
	if (!ft_strncmp(cmd, "exit", 5))
		ft_exit(m_shell, lexes);
	if (!ft_strncmp(cmd, "export", 7))
		ft_export(lexes, envs, &(*m_shell)->envps, ipc);
	if (!ft_strncmp(cmd, "pwd", 4))
		ft_pwd();
	if (!ft_strncmp(cmd, "unset", 6))
		ft_unset(lexes, envs, &(*m_shell)->envps, ipc);
	return (free(cmd), 0);
}

void	ft_exe(t_minishell **minishell, t_lexems *lexem, t_envs **envs,
		int ipc)
{
	char	*cmd;

	if (lexem->type == SEPERATOR)
		lexem = lexem->next;
	if (!lexem)
		exit(EXIT_FAILURE);
	// ft_redirections(lexem);
	// ft_heredoc(lexem);
	if (!ft_builtin(minishell, lexem, envs, ipc))
	{
		cmd = ft_getpath(lexem->value, (*minishell)->envps, true);
		if (!cmd)
			exit(INVALID_CMD);
		ft_execute(minishell, cmd, lexem);
	}
}

void	ft_close_write(int wr)
{
	if (wr != -1)
		close(wr);
}

void	ft_close_read(int re)
{
	if (re != -1)
		close(re);
}

void	ft_redirect_pipe(int number, int *fd, int *read, int i)
{
	if (*read != -1)
	{
		dup2(*read, STDIN_FILENO);
		close(*read);
		*read = -1;
	}
	if (i < number)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		fd[1] = -1;
	}
}

void	ft_child(t_minishell **minishell, int *read, int i, int ipc)
{
	int	*fds;

	fds = (*minishell)->pipe;
	ft_redirect_pipe((*minishell)->nbr_pipes, fds, read, i);
	ft_close_read(fds[0]);
	ft_close_read(*read);
	ft_close_write(fds[1]);
	ft_exe(minishell, (*minishell)->table[i], &(*minishell)->envs, ipc);
	exit(EXIT_FAILURE);
}

void	ft_parent(t_minishell **minishell, int *read, int *fd, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*minishell)->exit_status = WEXITSTATUS(status);
	else
		(*minishell)->exit_status = INVALID_CMD;
	ft_close_read(*read);
	ft_close_write(fd[1]);
	*read = fd[0];
}

int	ft_execute_commands(t_minishell **minishell)
{
	int		i;
	int		new_read;
	pid_t	pid;

	i = 0;
	if (pipe((*minishell)->ipc) == -1)
		perror("pipe error");
	new_read = -1;
	while ((*minishell)->table[i])
	{
		if (i < (*minishell)->nbr_pipes && pipe((*minishell)->pipe) == -1)
			perror("pipe error");
		pid = fork();
		if (pid == 0)
		{
			close((*minishell)->ipc[0]);
			ft_set_execution_sig();
			ft_child(minishell, &new_read, i, (*minishell)->ipc[1]);
		}
		else
		{
			close((*minishell)->ipc[1]);
			ft_parent(minishell, &new_read, (*minishell)->pipe, pid);
			ft_transfer_child_parent(&(*minishell)->envs, (*minishell)->ipc[0]);
			ft_init_sig();
		}
		i++;
	}
	return (close((*minishell)->ipc[0]), 1);
}
