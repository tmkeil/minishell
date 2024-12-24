/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/24 16:16:51 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait_for_child(t_minishell **minishell, int pid, char *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*minishell)->exit_status = WEXITSTATUS(status);
	else
	{
		(*minishell)->exit_status = INVALID_CMD;
		ft_printf("zsh: command failed: %s\n", cmd);
	}
}

int	ft_execute(t_minishell **minishell, char *cmd, char **envp)
{
	t_lexems	*token;
	char		**args;
	int			i;

	token = (*minishell)->tokens;
	args = malloc(sizeof(char *) * (ft_size(token) + 1));
	if (!args)
		exit(EXIT_FAILURE);
	i = 0;
	while (token)
	{
		args[i] = NULL;
		if (ft_strncmp((char *)token->value, " ", 1) != 0)
			ft_handle_lexem(args, i++, (char *)token->value, token->type, (*minishell)->envs);
		token = token->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
	{
		perror("execve");
		ft_free_ptr(&args);
		exit(EXIT_FAILURE);
	}
	return (ft_free_ptr(&args), EXIT_SUCCESS);
}

int	ft_builtin(t_minishell **minishell, int index)
{
	if (ft_changedir(minishell, (*minishell)->table[index]))
		return (1);
	if (ft_echo(minishell, (*minishell)->table[index]))
		return (1);
	if (ft_env((*minishell)->table[index], (*minishell)->envs))
		return (1);
	if (ft_exit(minishell, (*minishell)->table[index], index))
		return (1);
	if (ft_export((*minishell)->table[index], &(*minishell)->envs))
		return (1);
	if (ft_pwd((*minishell)->table[index]))
		return (1);
	if (ft_unset((*minishell)->table[index], &(*minishell)->envs))
		return (1);
	return (0);
}

void	ft_exe(t_minishell **minishell, char **envp, int index)
{
	char	*cmd;
	int		pid;

	if (ft_builtin(minishell, index))
	{
		// printf("test1\n");
		(*minishell)->exit_status = EXIT_SUCCESS;
	}
	else
	{
		// printf("test2\n");
		cmd = ft_getpath((*minishell)->table[index]->value, envp);
		pid = fork();
		if (pid == 0)
			ft_execute(minishell, cmd, envp);
		else if (pid > 0)
			ft_wait_for_child(minishell, pid, cmd);
		free(cmd);
	}
}

int	ft_execute_commands(t_minishell **minishell, char **envp)
{
	int		i;

	i = 0;
	while ((*minishell)->table[i])
	{
		ft_exe(minishell, envp, i);
		i++;
	}
	return (1);
}
