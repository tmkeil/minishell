/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 21:18:12 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_check_builtin(int index_lexem, char **envp, t_minishell **minishell)
{
	(void)envp;
	if (ft_changedir(minishell, (*minishell)->table[index_lexem]))
		return (1);
	if (ft_export((*minishell)->table[index_lexem], &(*minishell)->envs))
		return (1);
	if (ft_unset((*minishell)->table[index_lexem], &(*minishell)->envs))
		return (1);
	ft_exit((*minishell)->table[index_lexem], index_lexem, minishell);
	return (0);
}

int	ft_execute_commands(t_minishell **minishell, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;

	i = 0;
	while ((*minishell)->table[i])
	{
		if (ft_check_builtin(i, envp, minishell))
		{
			(*minishell)->exit_status = EXIT_SUCCESS;
			i++;
			continue ;
		}
		cmd = ft_getpath((*minishell)->table[i]->value, envp);
		if (!cmd)
			return (ft_handle_invalid_command(minishell, (*minishell)->table[i++]), 0);
		pid = fork();
		if (pid == 0)
			ft_execute(minishell, cmd, envp);
		else if (pid > 0)
			ft_wait_for_child(minishell, pid, cmd);
		free(cmd);
		i++;
	}
	return (1);
}
