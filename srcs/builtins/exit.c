/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:05:49 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 15:15:30 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ft_atol(char *s)
{
	size_t	i;
	int		p;
	long	val;
	bool	valid;

	i = 0;
	p = 1;
	val = 0;
	valid = false;
	while (s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i++] == '-')
			p = -1;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		valid = true;
		val = val * 10 + (s[i++] - '0');
	}
	if (!valid || s[i] != '\0')
		return (LONG_MIN);
	return (val * p);
}

int ft_exit(t_minishell **minishell, char **args)
{
	if (!args[1])
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit((*minishell)->exit_status);
	}
	if (ft_atol(args[1]) == LONG_MIN)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		ft_putstr_fd("bash: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (ft_ptrsize(args) > 2)
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
