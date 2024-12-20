/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 17:00:00 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_size(t_lexems *lexes)
{
	size_t	i;

	i = 0;
	while (lexes)
	{
		if (lexes->type != PIPE)
			i++;
		lexes = lexes->next;
	}
	return (i);
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
		handle_lexem(args, i++, (char *)token->value, token->type, (*minishell)->envs);
		token = token->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
	{
		perror("execve");
		ft_clr(&args);
		exit(EXIT_FAILURE);
	}
	return (ft_clr(&args), EXIT_SUCCESS);
}

int	ft_check_builtin(t_lexems *lexems, char **envp, t_envs **envp_list)
{
	(void)envp;
	if (ft_changedir(lexems))
		return (1);
	if (ft_handle_export(lexems, envp_list))
		return (1);
	if (ft_unset(lexems, envp_list))
		return (1);
	return (0);
}

void	handle_invalid_command(t_minishell **minishell, t_lexems *cmd)
{
	(*minishell)->exit_status = INVALID_CMD;
	printf("zsh: command not found: %s\n", (char *)cmd->value);
}

void	wait_for_child_process(t_minishell **minishell, int pid, char *cmd)
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

int	execute_commands(t_minishell **minishell, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;

	i = 0;
	while ((*minishell)->table[i])
	{
		if (ft_check_builtin((*minishell)->table[i], envp, &(*minishell)->envs))
		{
			(*minishell)->exit_status = EXIT_SUCCESS;
			i++;
			continue ;
		}
		cmd = ft_getpath((*minishell)->table[i]->value, envp);
		if (!cmd)
			return (handle_invalid_command(minishell, (*minishell)->table[i++]), 0);
		pid = fork();
		if (pid == 0)
			ft_execute(minishell, cmd, envp);
		else if (pid > 0)
			wait_for_child_process(minishell, pid, cmd);
		free(cmd);
		i++;
	}
	return (1);
}
