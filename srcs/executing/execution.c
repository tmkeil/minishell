/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 16:00:14 by tkeil            ###   ########.fr       */
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

void	ft_execute(t_lexems *lexems, char *cmd, char **envp, t_envs *envp_list)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (ft_size(lexems) + 1));
	if (!args)
		return ;
	i = 0;
	while (lexems)
	{
		args[i] = NULL;
		handle_lexem(args, i++, (char *)lexems->value, lexems->type, envp_list);
		lexems = lexems->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
	{
		ft_clr(&args);
		return ;
	}
}

void ft_first_word_exit_command(t_lexems *lexems, int index_lexem, t_minishell **minishell)
{
	if (index_lexem == 0 && ft_strncmp(lexems->value, "exit", 5) == 0)
	{
		if ((*minishell)->exit_status)
			exit((*minishell)->exit_status);
		else
			exit(0);
	}
}

int	ft_check_builtin(int index_lexem, char **envp, t_minishell **minishell)
{
	(void)envp;
	if (ft_changedir((*minishell)->table[index_lexem]))
		return (1);
	if (ft_handle_export((*minishell)->table[index_lexem], &(*minishell)->envs))
		return (1);
	if (ft_unset((*minishell)->table[index_lexem], &(*minishell)->envs))
		return (1);
	ft_first_word_exit_command((*minishell)->table[index_lexem], index_lexem, minishell);
	return (0);
}

int	execute_commands(t_minishell **minishell, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;
	bool	valid;

	i = 0;
	pid = 1;
	cmd = NULL;
	while ((*minishell)->table[i])
	{
		valid = false;
		if (ft_check_builtin(i, envp, minishell))
		{
			i++;
			continue ;
		}
		cmd = ft_getpath((*minishell)->table[i]->value, envp);
		if (cmd)
		{
			pid = fork();
			if (pid == 0)
				ft_execute((*minishell)->table[i], cmd, envp,
					(*minishell)->envs);
			valid = true;
		}
		if (pid > 0)
		{
			i++;
			free(cmd);
			waitpid(pid, NULL, 0);
		}
		if (!valid)
		{
			return (printf("zsh: command not found: %s\n",
					(char *)(*minishell)->table[i - 1]->value), INVALID_CMD);
		}
	}
	return (0);
}
