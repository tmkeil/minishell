/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/17 21:32:49 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_size(t_lexems *lexes)
{
	size_t	i;

	i = 0;
	while (lexes)
	{
		if (lexes->type == WORD || lexes->type == DOUBLE_QUOTE
			|| lexes->type == SINGLE_QUOTE)
			i++;
		lexes = lexes->next;
	}
	return (i);
}

char	*ft_find_end(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && (ft_isalpha(ptr[i]) || ptr[i] == '$'))
		i++;
	return (&ptr[i]);
}

char	*ft_until_next_env(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && ptr[i] != '$')
		i++;
	return (&ptr[i]);
}

void	ft_execute(t_lexems *lexems, char *cmd, char **envp, int pid)
{
	char	**args;
	int		i;
	char	*ptr;
	char	*env_var;
	char	*current;
	int		len;
	char	*sub;
	char	*result;
	char	*v;
	int		p;
	int		dollar_count;

	args = malloc(sizeof(char *) * (ft_size(lexems) + 1));
	if (!args)
		return ;
	i = 0;
	len = 0;
	p = 0;
	while (lexems)
	{
		args[i] = NULL;
		current = (char *)(lexems->value);
		result = NULL;
		dollar_count = 0;
		// printf("current = %s\n", current);
		while (*current)
		{
			if (*current != '$')
			{
				dollar_count++;
				len = ft_until_next_env(current) - current;
				printf("len = %i\n", len);
				sub = ft_substr(current, 0, len);
				args[i] = ft_strjoin(args[i], sub);
				current += len;
			}
			else
			{
				ptr = ft_strchr(current, '$');
				if (ptr)
				{
					len = ft_find_end(ptr) - ptr;
					printf("var len = %i\n", len);
					env_var = ft_substr(ptr, 0, len);
					printf("env var name = %s\n", env_var);
					v = getenv(env_var + 1);
					if (!v)
					{
						printf("env gibt es nicht: %s\n", env_var + 1);
						p = 0;
						while (env_var[p] && env_var[p + 1] == '$')
						{
							args[i] = ft_strjoin(args[i], ft_itoa(pid));
							p += 2;
						}
						if (env_var[p])
							args[i] = ft_strjoin(args[i], env_var[p]);
						
						// args[i] = ft_strjoin(args[i], "");
						current += len;
					}
					else
					{
						// if (!v)
						// {
						//  free(env_var);
						//  return ;
						// }
						printf("env gibt es: %s\n", env_var + 1);
						args[i] = ft_strjoin(args[i], v);
						current += len;
					}
				}
				else
					current++;
			}
		}
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
			ft_execute(exec_table->lexems[i], cmd, envp, pid);
		}
	}
	free(cmd);
	waitpid(pid, NULL, 0);
	// else if ()
	// {
	//  /* code */
	// }
	return (2);
}
