/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/30 15:15:40 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_only_minus_n(t_lexems *lexem)
{
	int		i;
	char	*current;

	i = 2;
	current = (char *)lexem->value;
	if (ft_strncmp(current, "-n", 2) != 0)
		return (0);
	if (!ft_strncmp(current, "-n", 2))
	{
		while (current[i])
		{
			if (current[i] != 'n')
				return (0);
			i++;
		}
	}
	return (1);
}

int	ft_minus_n(t_lexems **lexem, bool *nl)
{
	if (ft_is_only_minus_n(*lexem))
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
		else if (!ft_strncmp((char *)(*lexem)->value, "-n", 2))
			return (2);
	}
	return (1);
}

int	ft_echo(t_lexems *lexem)
{
	int		minusn;
	bool	nl;

	nl = true;
	
	if (!lexem->next)
		return (write(STDOUT_FILENO, "\n", 1), 2);
	lexem = lexem->next->next;
	while (1)
	{
		minusn = ft_minus_n(&lexem, &nl);
		if (minusn == 1)
			break ;
		if (!minusn)
			return (1);
	}
	while (lexem)
	{
		ft_putstr_fd((char *)lexem->value, STDOUT_FILENO);
		lexem = lexem->next;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	return (1);
}
