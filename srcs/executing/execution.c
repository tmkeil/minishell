/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/26 15:57:56 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_wait_for_child(t_minishell **minishell, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*minishell)->exit_status = WEXITSTATUS(status);
	else
		(*minishell)->exit_status = INVALID_CMD;
}

int	ft_execute(t_minishell **minishell, char *cmd, char **envp, char *prompt)
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
		ft_putstr_fd(prompt, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		ft_free_ptr(&args);
		exit(INVALID_CMD);
	}
	return (ft_free_ptr(&args), EXIT_SUCCESS);
}

int	ft_builtin(t_minishell **minishell, t_lexems *lexes)
{
	if (ft_changedir(minishell, lexes))
		return (1);
	if (ft_echo(minishell, lexes))
		return (1);
	if (ft_env(lexes, (*minishell)->envs))
		return (1);
	if (ft_exit(minishell, lexes))
		return (1);
	if (ft_export(lexes, &(*minishell)->envs))
		return (1);
	if (ft_pwd(lexes))
		return (1);
	if (ft_unset(lexes, &(*minishell)->envs))
		return (1);
	return (0);
}

void	ft_exe(t_minishell **minishell, t_lexems *lexes, char **envp)
{
	char		*cmd;
	int			pid;

	if (ft_builtin(minishell, lexes))
		(*minishell)->exit_status = EXIT_SUCCESS;
	else
	{
		cmd = ft_getpath(lexes->value, envp);
		pid = fork();
		if (pid == 0)
			ft_execute(minishell, cmd, envp, (char *)lexes->value);
		else if (pid > 0)
			ft_wait_for_child(minishell, pid);
		free(cmd);
	}
}

int	ft_execute_commands(t_minishell **minishell, char **envp)
{
	int			i;
	t_lexems	**lexes;

	i = 0;
	lexes = (*minishell)->table;
	while (lexes[i])
	{
		ft_exe(minishell, lexes[i], envp);
		i++;
	}
	return (1);
}
