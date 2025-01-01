/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/01 15:45:12 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_only_minus_n(char *current, bool absolute)
{
	int		i;

	i = 2;
	if (!ft_strncmp(current, "-n", 3) && absolute)
		return (1);
	if (!ft_strncmp(current, "-n", 2))
	{
		while (current[i])
		{
			if (current[i] != 'n')
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int	ft_minus_n(t_lexems **lexem, bool *nl, bool absolute)
{
	if (ft_is_only_minus_n((*lexem)->value, absolute))
	{
		if (!(*lexem)->next)
			return (0);
		if ((*lexem)->next->type == SEPERATOR)
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

void	ft_run_absolute_echo(char ***args)
{
	int		i;
	bool	nl;

	if (!*args || !**args || !***args)
		exit(EXIT_FAILURE);
	i = 0;
	nl = true;
	while (*args[++i])
	{
		if (i == 1 && ft_is_only_minus_n(*args[i], true))
		{
			nl = false;
			continue ;
		}
		ft_putstr_fd(*args[i], STDOUT_FILENO);
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	ft_free_ptr(args);
	exit(EXIT_SUCCESS);
}

void	ft_if_no_args(t_lexems *lexem)
{
	if (!lexem->next)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(EXIT_SUCCESS);
	}
}

void	ft_echo(t_lexems *lexem, char ***args, bool absolute)
{
	int		minusn;
	bool	nl;

	nl = true;
	if (absolute)
		ft_run_absolute_echo(args);
	ft_if_no_args(lexem);
	lexem = lexem->next->next;
	while (1)
	{
		minusn = ft_minus_n(&lexem, &nl, absolute);
		if (!minusn)
			exit(EXIT_SUCCESS);
		if (minusn == 1 || absolute)
			break ;
	}
	while (lexem)
	{
		ft_putstr_fd((char *)lexem->value, STDOUT_FILENO);
		lexem = lexem->next;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	exit(EXIT_SUCCESS);
}
