/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getpath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:49:09 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 16:37:29 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (ft_free_ptr(&env), NULL);
		if (access(full, X_OK) == 0)
			return (ft_free_ptr(&env), full);
		free(full);
		i++;
	}
	return (ft_free_ptr(&env), NULL);
}

void	ft_to_lower(char **cmd)
{
	int	i;

	i = 0;
	while ((*cmd)[i])
	{
		(*cmd)[i] = ft_tolower((*cmd)[i]);
		i++;
	}
}

char	*ft_get_relative_path(char *cmd, char ***args)
{
	char	*rel;
	char	*tmp;
	char	curr[1024];

	getcwd(curr, 1024);
	rel = ft_strjoin(curr, "/");
	tmp = rel;
	rel = ft_strjoin(tmp, cmd);
	free(tmp);
	free(*args[0]);
	*args[0] = ft_strdup(rel);
	return (rel);
}

char	*ft_getpath(char *cmd, char **envp, char ***args)
{
	char	**ptr;
	char	**env;

	if (*cmd == '.')
		return (ft_get_relative_path(cmd, args));
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (!envp || !*envp)
		return (NULL);
	ptr = envp;
	while (*ptr)
	{
		if (ft_strnstr(*ptr, "PATH=", 5))
			break ;
		ptr++;
	}
	if (!*ptr)
		return (NULL);
	env = ft_split(*ptr + 5, ':');
	if (!env || !*env)
		return (NULL);
	return (ft_check_paths(env, cmd));
}
