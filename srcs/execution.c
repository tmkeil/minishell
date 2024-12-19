/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/19 14:45:39 by tkeil            ###   ########.fr       */
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

int	ft_check_builtin(t_lexems *lexems, char **envp)
{
	if (ft_changedir(lexems))
		return (1);
	if (ft_handle_export(lexems, envp))
		return (1);
	if (ft_unset(lexems, envp))
		return (1);
	return (0);
}

int	execute_commands(t_lexems **table, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;
	bool	valid;

	i = 0;
	pid = 1;
	cmd = NULL;
	while (table[i])
	{
		valid = false;
		if (ft_check_builtin(table[i], envp))
		{
			i++;
			continue ;
		}
		cmd = ft_getpath(table[i]->value, envp);
		if (cmd)
		{
			pid = fork();
			if (pid == 0)
				ft_execute(table[i], cmd, envp);
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
			printf("\n");
			return (printf("zsh: command not found: %s\n", (char *)table[i - 1]->value), INVALID_CMD);
		}
	}
	return (0);
}
