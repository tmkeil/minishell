/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 14:31:11 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_lexems *lexem)
{
	bool	nl;

	nl = true;
	if (!lexem->next)
		return (write(STDOUT_FILENO, "\n", 1), 2);
	lexem = lexem->next->next;
	if (!ft_strncmp(lexem->value, "-n", 2))
	{
		if (lexem->next)
			lexem = lexem->next->next;
		else
			return (1);
		nl = false;
	}
	while (lexem)
	{
		if (lexem->type != INVALID)
			ft_putstr_fd((char *)lexem->value, STDOUT_FILENO);
		lexem = lexem->next;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}
