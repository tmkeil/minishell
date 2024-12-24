/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/24 16:16:24 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_minishell **minishell, t_lexems *lexem)
{
	// printf("vv\n");
	// printf("lexem value is %s\nlexem len = %li\n", (char *)lexem->value, ft_strlen((char *)lexem->value));
	if (!lexem || !lexem->next || ft_strncmp(lexem->value, "echo", 4) != 0)
		return (0);
	// printf("vv2\n");
	lexem = lexem->next->next;
	while (lexem)
	{
		if (lexem->type != INVALID)
		{
			// printf("lexem value is %s\nlexem len = %li\n", (char *)lexem->value, ft_strlen((char *)lexem->value));
			write(STDOUT_FILENO, (char *)lexem->value,
				ft_strlen((char *)lexem->value));
		}
		lexem = lexem->next;
	}
	(void)minishell;
	write(STDOUT_FILENO, "\n", 1);
	return (1);
}
