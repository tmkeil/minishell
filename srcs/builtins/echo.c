/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 21:33:53 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_minus_n(t_lexems **lexem, bool *nl)
{
	if (!ft_strncmp((char *)(*lexem)->value, "-n", 3))
	{
		if (!(*lexem)->next)
			return (0);
		if (!ft_strncmp((*lexem)->next->value, " ", 1))
		{
			*nl = false;
			(*lexem) = (*lexem)->next->next;
		}
		if (!(*lexem))
			return (0);
	}
	return (1);
}

int	ft_echo(t_lexems *lexem)
{
	bool	nl;

	nl = true;
	if (!lexem->next)
		return (write(STDOUT_FILENO, "\n", 1), 2);
	lexem = lexem->next->next;
	if (!ft_minus_n(&lexem, &nl))
		return (1);
	while (lexem)
	{
		ft_putstr_fd((char *)lexem->value, STDOUT_FILENO);
		lexem = lexem->next;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}
