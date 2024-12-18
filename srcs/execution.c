/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 18:38:34 by tkeil            ###   ########.fr       */
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

void	ft_execute(t_lexems *lexems, char *cmd, char **envp)
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
		handle_lexem(args, i++, (char *)lexems->value, lexems->type);
		lexems = lexems->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
		clean_args(args);
	clean_args(args);
}

int ft_check_builtin(t_lexems *lexems)
{
	if (ft_changedir(lexems))
		return (1);
	return (0);
}

int	execute_commands(t_exec_table *exec_table, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;

	i = 0;
	pid = 0;
	cmd = NULL;
	while (exec_table->lexems[i])
	{
		if (ft_check_builtin(exec_table->lexems[i]))
		{
			i++;
			continue ;
		}
		cmd = ft_getpath(exec_table->lexems[i]->value, envp);
		if (cmd)
		{
			pid = fork();
			if (pid == 0)
				ft_execute(exec_table->lexems[i], cmd, envp);
		}
		if (pid > 0)
		{
			i++;
			free(cmd);
			waitpid(pid, NULL, 0);
		}
	}
	return (2);
}
