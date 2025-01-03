/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 16:01:08 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_update_pwd(t_envs **envs)
{
	char    buffer[1024];
	
	if (!getcwd(buffer, sizeof(buffer)))
		return (perror("getcwd"), 0);
	ft_set_env("OLDPWD", ft_get_env("PWD", *envs), envs);
	ft_set_env("PWD", buffer, envs);
	return (1);
}

int	ft_cd_to(t_lexems *lexems, char **path)
{
	char	*home;

	home = getenv("HOME");
	if (!lexems->next || !lexems->next->next)
		return (*path = ft_strdup(home), 1);
	lexems = lexems->next->next;
	*path = ft_strdup((char *)lexems->value);
	if (lexems->next)
	{
		if (lexems->next->next)
			return (0);
	}
	return (1);
}

void	ft_changedir(t_minishell **minishell, t_lexems *lexems)
{
	char	*path;

	path = NULL;
	if (!ft_cd_to(lexems, &path))
	{
		ft_put_error_str(CD_TOO_MANY, NULL);
		free(path);
		exit(EXIT_FAILURE);
	}
	if (chdir(path) != 0)
	{
		ft_put_error_str(BAD_CD, path);
		free(path);
		exit(EXIT_FAILURE);
	}
	free(path);
	ft_update_pwd(&(*minishell)->envs);
	ft_update_envps((*minishell)->envs, &(*minishell)->envps);
	exit(EXIT_SUCCESS);
}
