/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getpath.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:49:09 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/30 16:15:02 by tkeil            ###   ########.fr       */
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

char	*ft_getpath(char *cmd, char **envp, bool absolute)
{
	char	**ptr;
	char	**env;

	if (absolute)
		ft_to_lower(&cmd);
	if (access(cmd, X_OK) == 0 && absolute)
	{
		return (ft_strdup(cmd));
	}
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
