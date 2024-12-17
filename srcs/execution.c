/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/17 17:39:15 by tkeil            ###   ########.fr       */
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

size_t find_prefix_length(const char *ptr)
{
    size_t i = 0;
    while (ptr[i] && ptr[i] != '$')
        i++;
    return i;
}

char *get_prefix(const char *ptr, size_t length)
{
    char *prefix = malloc(length + 1);
    if (!prefix)
        return NULL;
    ft_strlcpy(prefix, ptr, length + 1);
    return prefix;
}

int is_valid_env_char(char c)
{
    return ft_isalnum(c) || c == '_';
}

char *handle_lone_dollar(char *prefix, size_t *counter, size_t length)
{
    char *result = ft_strjoin(prefix, "$");
    free(prefix);
    *counter += length + 1;
    return result;
}

char *get_env_name(const char *ptr, size_t start, size_t *end)
{
    size_t j = start;
    while (ptr[j] && is_valid_env_char(ptr[j]))
        j++;
    size_t var_len = j - start;
    char *var_name = malloc(var_len + 1);
    if (!var_name)
        return NULL;
    ft_strlcpy(var_name, ptr + start, var_len + 1);
    *end = j;
    return var_name;
}

char *replace_with_env_value(char *prefix, const char *env_name)
{
    char *env_value = getenv(env_name);
    if (!env_value)
        env_value = "";
    char *result = ft_strjoin(prefix, env_value);
    free(prefix);
    return result;
}

char *ft_until_next_env(char *ptr, size_t *counter)
{
    size_t i = find_prefix_length(ptr);
    char *prefix = get_prefix(ptr, i);
    if (!prefix)
        return NULL;

    if (ptr[i] != '$')
    {
        *counter += i;
        return prefix;
    }
    if (!ptr[i + 1] || !is_valid_env_char(ptr[i + 1]))
        return handle_lone_dollar(prefix, counter, i);
    size_t j;
    char *env_name = get_env_name(ptr, i + 1, &j);
    if (!env_name)
    {
        free(prefix);
        return NULL;
    }
    char *result = replace_with_env_value(prefix, env_name);
    free(env_name);
    *counter += j;
    return result;
}

char *substitute_dollar_values_and_join(char *str)
{
    char *final_str = ft_strdup("");
    char *temp;
    char *next_part;
    size_t counter = 0;

    if (!final_str)
        return NULL;
    while (str[counter])
    {
        next_part = ft_until_next_env(str + counter, &counter);
        if (!next_part)
        {
            free(final_str);
            return NULL;
        }
        temp = final_str;
        final_str = ft_strjoin(final_str, next_part);
        free(temp);
        free(next_part);
    }
    return final_str;
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
		if (ft_strchr(lexems->value,'$'))
		{
			args[i] = substitute_dollar_values_and_join(lexems->value);
		}
		else
		{
			args[i] = ft_strdup(lexems->value);
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
