/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/23 17:51:49 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_minishell **minishell, t_lexems *lexem)
{
	if (!lexem || ft_strncmp(lexem->value, "echo", 4) != 0)
		return (0);
	while (lexem)
	{
		if (!ft_strnstr(OPERATIONS, (char *)lexem->value, ft_strlen(OPERATIONS))
			&& lexem->type != INVALID)
			write(STDOUT_FILENO, (char *)lexem->value,
				ft_strlen((char *)lexem->value));
		lexem = lexem->next;
	}
	(void)minishell;
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
