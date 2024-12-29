/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:05:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 16:46:28 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ft_atol(char *s)
{
	size_t	i;
	int		p;
	long	val;
	bool	valid;

	if (!s)
		return (255);
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
		if (val > 255)
			val = 255;
	}
	if (!valid || s[i] != '\0')
		return (255);
	return (val * p);
}

int check_if_more_arguments(t_lexems *lexems)
{
	t_lexems *current;

	current = lexems->next;
	while(current)
	{
		if (ft_isalnum(((char *)(current->value))[0]))
			return (1);
		current = current->next;
	}
	return (0);
}

int ft_exit(t_minishell **minishell, t_lexems *lexems)
{
	int value_exit;

	if (!lexems->next || !lexems->next->next)
	{
		if (*minishell)
			ft_free_shell(minishell);
		exit((*minishell)->exit_status);
	}
	lexems = lexems->next->next;
	value_exit = (int)ft_atol(lexems->value);
	if (value_exit < 0 || value_exit > 255)
		value_exit = value_exit % 256;
	if (value_exit == 255 && !ft_isdigit(((char *)(lexems->value))[0]))
	{
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(lexems->value, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required", STDERR_FILENO);
	}
	if (check_if_more_arguments(lexems))
		ft_putstr_fd("exit: too many arguments", STDERR_FILENO);
	if (*minishell)
		ft_free_shell(minishell);
	exit(value_exit);
}
