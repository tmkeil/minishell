/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/17 22:41:04 by tkeil            ###   ########.fr       */
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
	char		**args;
	int			i;
	size_t		len;
	char		*current;
	char		*result;
	char		*sub;
	char		*env_var;
	const char	*v;
	char		*ptr;
	char		*temp;
	char		*pid_str;

	args = malloc(sizeof(char *) * (ft_size(lexems) + 1));
	if (!args)
		return ;
	i = 0;
	while (lexems)
	{
		current = (char *)(lexems->value);
		result = NULL;
		while (*current)
		{
			if (*current != '$')
			{
				len = ft_until_next_env(current) - current;
				sub = ft_substr(current, 0, len);
				if (!result)
					result = sub;
				else
				{
					temp = ft_strjoin(result, sub);
					free(result);
					result = temp;
				}
				free(sub);
				current += len;
			}
			else
			{
				ptr = current + 1;
				if (*ptr == '$')
				{
					pid_str = ft_itoa(pid);
					if (!result)
						result = ft_strdup(pid_str);
					else
					{
						temp = ft_strjoin(result, pid_str);
						free(result);
						result = temp;
					}
					free(pid_str);
					current += 2;
				}
				else if (ft_isalpha(*ptr) || *ptr == '_')
				{
					len = ft_find_end(current + 1) - (current + 1);
					env_var = ft_substr(current + 1, 0, len);
					v = getenv(env_var);
					if (v)
					{
						if (!result)
							result = ft_strdup(v);
						else
						{
							temp = ft_strjoin(result, v);
							free(result);
							result = temp;
						}
					}
					free(env_var);
					current += len + 1;
				}
				else
				{
					if (!result)
						result = ft_strdup("$");
					else
					{
						temp = ft_strjoin(result, "$");
						free(result);
						result = temp;
					}
					current++;
				}
			}
		}
		args[i++] = result;
		lexems = lexems->next;
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
