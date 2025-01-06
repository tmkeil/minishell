/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:29:09 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 17:46:17 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_is_builtin(void *value, char **envp)
{
	int		i;
	char	*path;
	char	**cmd_path;

	if (!value)
		return (NULL);
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

int	ft_choose_builtin(t_minishell **minishell, char *cmd_builtin, char **args, int )
{
	if (!ft_strncmp(cmd_builtin, "cd", 3))
		return (ft_changedir(minishell, args));
	if (!ft_strncmp(cmd_builtin, "echo", 5))
		return (ft_echo(args));
	if (!ft_strncmp(cmd_builtin, "env", 4))
		return (ft_env((*minishell)->envs));
	if (!ft_strncmp(cmd_builtin, "exit", 5))
		return (ft_exit(minishell, args));
	if (!ft_strncmp(cmd_builtin, "export", 7))
		return (ft_export(minishell, args, &(*minishell)->envs));
	if (!ft_strncmp(cmd_builtin, "pwd", 4))
		return (ft_pwd());
	if (!ft_strncmp(cmd_builtin, "unset", 6))
		return (ft_unset(minishell, args, &(*minishell)->envs));
	return (-2);
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
