/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/27 20:32:59 by tkeil            ###   ########.fr       */
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

void	ft_failed(t_minishell **minishell, char *msg, void *path)
{
	(*minishell)->exit_status = INVALID_CD;
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd((char *)path, STDERR_FILENO);
}

void	ft_too_many(t_minishell **minishell, char *msg)
{
	(*minishell)->exit_status = INVALID_CD;
	ft_putendl_fd(msg, STDERR_FILENO);
}

int	ft_cd_to(t_lexems *lexems, char **path)
{
	int		i;
	char	*home;

	i = 0;
	home = getenv("HOME");
	if (!lexems->next || !lexems->next->next)
		return (*path = ft_strdup(home), 1);
	else if (lexems->next->next)
	{
		if (((char *)lexems->value)[0] == '~')
		{
			*path = ft_strdup(home);
			if (!path && ++i)
				return (0);
		}
		lexems = lexems->next->next;
		while (((char *)lexems->value)[i])
			ft_strappend(path, ((char *)lexems->value)[i++]);
	}
	if (lexems->next)
	{
		if (lexems->next->next)
			return (0);
	}
	return (1);
}

int	ft_changedir(t_minishell **minishell, t_lexems *lexems)
{
	char	*path;

	path = NULL;
	if (!ft_cd_to(lexems, &path))
		return (ft_too_many(minishell, CD_TOO_MANY), free(path), 2);
	if (chdir(path) != 0)
		return (ft_failed(minishell, BAD_CD, path), free(path), 2);
	free(path);
	return (ft_update_pwd(&(*minishell)->envs));
}
