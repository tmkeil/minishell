/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/29 13:08:38 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(t_minishell **minishell, char *cmd, char **envp, char *prompt)
{
	t_lexems	*token;
	char		**args;
	size_t		size;

	token = (*minishell)->tokens;
	size = ft_size(token);
	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		exit(EXIT_FAILURE);
	while (size + 1)
		args[size--] = NULL;
	if (ft_handle_lexem(&args, token, cmd) == 2)
		return (ft_free_ptr(&args), EXIT_SUCCESS);
	if (execve(cmd, args, envp) == -1)
	{
		ft_putstr_fd(prompt, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free_ptr(&args);
		exit(INVALID_CMD);
	}
	return (ft_free_ptr(&args), EXIT_SUCCESS);
}

char	*ft_is_builtin(t_minishell *minishell, void *value, char **envp)
{
	int		i;
	char	*path;
	char	**cmd_path;

	(void)minishell;
	if (ft_strnstr(BUILTINS, (char *)value, ft_strlen(BUILTINS)))
		return (ft_strdup((char *)value));
	path = ft_getpath((char *)value, envp);
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

int	ft_builtin(t_minishell **minishell, t_lexems *lexes, t_envs **envs)
{
	int		status;
	char	*cmd;
	
	status = 0;
	if (!ft_strncmp((char *)lexes->value, " ", 1))
		lexes = lexes->next;
	cmd = ft_is_builtin(*minishell, lexes->value, (*minishell)->envps);
	if (!cmd)
		return (0);
	if (!ft_strncmp(cmd, "cd", 3))
		status = ft_changedir(minishell, lexes);
	if (!ft_strncmp(cmd, "echo", 5))
		status = ft_echo(lexes);
	if (!ft_strncmp(cmd, "env", 4))
		status = ft_env(*envs);
	if (!ft_strncmp(cmd, "exit", 5))
		status = ft_exit(minishell, lexes);
	if (!ft_strncmp(cmd, "export", 7))
		status = ft_export(minishell, lexes, envs, &(*minishell)->envps);
	if (!ft_strncmp(cmd, "pwd", 4))
		status = ft_pwd();
	if (!ft_strncmp(cmd, "unset", 6))
		status = ft_unset(minishell, lexes, envs, &(*minishell)->envps);
	return (free(cmd), status);
}

void	ft_exe(t_minishell **minishell, t_lexems *lexes, t_envs **envs)
{
	char		*cmd;
	int			pid;
	int			builtin;

	builtin = ft_builtin(minishell, lexes, envs);
	if (builtin == 1)
		(*minishell)->exit_status = EXIT_SUCCESS;
	else if (builtin == 0)
	{
		cmd = ft_getpath(lexes->value, (*minishell)->envps);
		pid = fork();
		if (pid == 0)
			ft_execute(minishell, cmd, (*minishell)->envps, (char *)lexes->value);
		else if (pid > 0)
			ft_wait_for_child(minishell, pid);
		free(cmd);
	}
}

int	ft_execute_commands(t_minishell **minishell)
{
	int			i;
	t_lexems	**lexes;

	i = 0;
	lexes = (*minishell)->table;
	while (lexes[i])
	{
		ft_exe(minishell, lexes[i], &(*minishell)->envs);
		i++;
	}
	return (1);
}
