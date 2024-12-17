/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/17 15:23:21 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_size(t_lexems *lexes)
{
	size_t	i;

	i = 0;
	while (lexes)
	{
		if (lexes->type == WORD || lexes->type == DOUBLE_QUOTE || lexes->type == SINGLE_QUOTE)
			i++;
		lexes = lexes->next;
	}
	return (i);
}

void clean_args(char **args)
{
	int i;

	i = 0;
	while(args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	ft_execute(t_lexems *lexems, char *cmd, char **envp)
{
	char	**args;
	int		i;
	int		size;

	size = ft_size(lexems);
	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		return ;
	i = 0;
	while (i < size)
	{
		// args[i] = malloc(sizeof(char) * (ft_strlen((char *)lexems->value)
		args[i] = ft_strdup((char *)lexems->value);
		lexems = lexems->next;
		i++;
	}
	args[i] = NULL;
	execve(cmd, args, envp);
	clean_args(args);
}

static char	*ft_check_paths(char **env, char *cmd)
{
	int		i;
	char	*path;
	char	*full;

	i = 0;
	path = NULL;
	full = NULL;
	while (env[i])
	{
		path = ft_strjoin(env[i], "/");
		full = ft_strjoin(path, cmd);
		free(path);
		if (!full)
			return (ft_clr(&env), NULL);
		if (access(full, X_OK) == 0)
			return (ft_clr(&env), full);
		free(full);
		i++;
	}
	return (ft_clr(&env), NULL);
}

char	*ft_getpath(char *cmd, char **envp)
{
	char	**env;

	if (!envp || !*envp)
		return (NULL);
	while (*envp)
	{
		if (ft_strnstr(*envp, "PATH=", 5))
			break ;
		envp++;
	}
	env = ft_split(*envp + 5, ':');
	if (!env || !*env)
		return (NULL);
	return (ft_check_paths(env, cmd));
}

int	execute_commands(t_exec_table *exec_table, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;

	i = 0;
	pid = 0;
	cmd = ft_getpath(exec_table->lexems[i]->value, envp);
	if (cmd)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_execute(exec_table->lexems[i], cmd, envp);
		}
	}
	free(cmd);
	waitpid(pid, NULL, 0);
	// else if ()
	// {
	// 	/* code */
	// }
	return (2);
}
