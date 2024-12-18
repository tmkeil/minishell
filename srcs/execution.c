/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 18:46:34 by tkeil            ###   ########.fr       */
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

void ft_handle_export(t_lexems *args, char **envp)
{
 	if (args == NULL || args->next == NULL || ft_strchr(args->next->value, '=') == NULL)
	{
        int i = 0;
        while (envp[i]) {
            ft_printf("%s\n", envp[i]);
            i++;
        }
        return;
    }
}

int ft_check_builtin(t_lexems *lexems, char **envp)
{
	if (ft_changedir(lexems))
		return (1);
	else if(ft_strncmp(lexems->value, "export", 7) == 0)
	{
		ft_handle_export(lexems, envp);
		return (1);
	}
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
		if (ft_check_builtin(exec_table->lexems[i], envp))
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
