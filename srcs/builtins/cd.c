/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 21:09:46 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_changedir(t_minishell **minishell, t_lexems *lexems)
{
	char	*home;

	home = NULL;
	if (lexems->type == WORD && !ft_strncmp((char *)lexems->value, "cd", 2))
	{
		if (lexems->next)
		{
			if (chdir((char *)lexems->next->value) != 0)
			{
				(*minishell)->exit_status = INVALID_CD;
				printf("cd: no such file or directory: %s\n",
					(char *)lexems->next->value);
			}
		}
		else
		{
			home = getenv("HOME");
			if (chdir(home) != 0)
			{
				(*minishell)->exit_status = INVALID_CD;
				printf("cd: no such file or directory: %s\n", home);
			}
		}
		return (1);
	}
	return (0);
}
