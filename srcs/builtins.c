/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 19:10:54 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_export(t_lexems *lexems, char **envp)
{
	int	i;

	if (ft_strncmp(lexems->value, "export", 7) == 0)
	{
		if (!lexems || !lexems->next || !ft_strchr(lexems->next->value, '='))
		{
			i = 0;
			while (envp[i])
			{
				ft_printf("%s\n", envp[i]);
				i++;
			}
			return (1);
		}
	}
	return (0);
}

int	ft_changedir(t_lexems *lexems)
{
	char	*home;

	home = NULL;
	if (lexems->type == WORD && !ft_strncmp((char *)lexems->value, "cd", 2))
	{
		if (lexems->next)
		{
			if (chdir((char *)lexems->next->value) != 0)
				printf("cd: no such file or directory: %s\n",
					(char *)lexems->next->value);
		}
		else
		{
			home = getenv("HOME");
			if (chdir(home) != 0)
				printf("cd: no such file or directory: %s\n", home);
		}
		return (1);
	}
	return (0);
}
