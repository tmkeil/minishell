/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/22 16:06:00 by tkeil            ###   ########.fr       */
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

void	ft_print_fail(char *msg, void *file)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd((char *)file, STDERR_FILENO);
}

int	ft_changedir(t_minishell **minishell, t_lexems *lexems)
{
	char	*home;

	home = NULL;
	if (ft_strncmp((char *)lexems->value, "cd", 2) != 0)
		return (0);
	if (lexems->next)
	{
		if (chdir((char *)lexems->next->value) != 0)
		{
			(*minishell)->exit_status = INVALID_CD;
			ft_print_fail(BAD_CD, lexems->next->value);
		}
	}
	else
	{
		home = getenv("HOME");
		if (chdir(home) != 0)
		{
			(*minishell)->exit_status = INVALID_CD;
			ft_print_fail(BAD_CD, home);
		}
	}
	return (ft_update_pwd(&(*minishell)->envs));
}
