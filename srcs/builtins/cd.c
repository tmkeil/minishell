/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 15:17:03 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_update_pwd(t_envs **envs)
{
	char	buffer[1024];

	if (!getcwd(buffer, sizeof(buffer)))
		return (perror("getcwd"), 0);
	ft_set_env("OLDPWD", ft_get_env("PWD", *envs), envs);
	ft_set_env("PWD", buffer, envs);
	return (1);
}

int	ft_cd_to(char *str, char **path)
{
	char	*home;

	home = getenv("HOME");
	if (!str)
		return (*path = ft_strdup(home), 1);
	return (*path = ft_strdup(str), 1);
}

int	ft_changedir(t_minishell **minishell, char **args)
{
	char	*path;

	path = NULL;
	ft_cd_to(args[1], &path);
	if (chdir(path) != 0)
	{
		ft_put_error_str(BAD_CD, path);
		free(path);
		return (EXIT_FAILURE);
	}
	free(path);
	ft_update_pwd(&(*minishell)->envs);
	ft_update_envps((*minishell)->envs, &(*minishell)->envps);
	return (EXIT_SUCCESS);
}
