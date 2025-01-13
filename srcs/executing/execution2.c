/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:29:09 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/11 22:27:31 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_is_builtin(void *value)
{
	if (!value)
		return (NULL);
	if (ft_strnstr(BUILTINS, (char *)value, ft_strlen(BUILTINS)))
		return (ft_strdup((char *)value));
	return (NULL);
}

int	ft_choose_builtin(t_minishell **minishell, char *cmd_builtin, char **args)
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
	return (-1);
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
