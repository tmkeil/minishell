/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:13:21 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 14:52:57 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ft_atol(char *s, int *index)
{
	int		p;
	long	val;
	bool	valid;

	p = 1;
	val = 0;
	valid = false;
	while (s[*index] && (s[*index] == ' ' || (s[*index] >= 9
				&& s[*index] <= 13)))
		(*index)++;
	if (s[*index] == '-' || s[*index] == '+')
	{
		if (s[*index] == '-')
			p = -1;
		(*index)++;
	}
	while (s[*index] >= '0' && s[*index] <= '9')
	{
		valid = true;
		val = val * 10 + (s[*index] - '0');
		(*index)++;
	}
	if (!valid || !(s[*index] >= 9 && s[*index] <= 13))
		return (LONG_MIN);
	return (val * p);
}

int check_array_size(char **array)
{
	int i;

	i = 0;
	while(array[i])
		i++;
	return (i);
}
