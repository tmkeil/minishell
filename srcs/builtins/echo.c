/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:54:34 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 15:12:51 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_only_minusn(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-')
		i++;
	else
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_jump(char **args, int *i)
{
	while (args[*i] && ft_is_only_minusn(args[*i]))
		(*i)++;
}

int	ft_echo(char **args)
{
	bool	nl;
	int		i;

	i = 1;
	nl = true;
	if (!args[1])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	if (args[i] && ft_is_only_minusn(args[i]))
	{
		nl = false;
		ft_jump(args, &i);
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (nl)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
