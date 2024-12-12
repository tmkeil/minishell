/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:42:41 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/19 19:42:41 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	ft_length(unsigned int nbr)
{
	size_t	i;

	i = 0;
	while (nbr)
	{
		nbr /= 16;
		i++;
	}
	return (i);
}

static int	ft_printhex(unsigned int nbr, char c)
{
	if (nbr >= 16)
	{
		if (ft_printhex(nbr / 16, c) == -1)
			return (-1);
		if (ft_printhex(nbr % 16, c) == -1)
			return (-1);
	}
	else
	{
		if (c == 'x')
		{
			if (ft_putchar("0123456789abcdef"[nbr]) == -1)
				return (-1);
		}
		else if (c == 'X')
		{
			if (ft_putchar("0123456789ABCDEF"[nbr]) == -1)
				return (-1);
		}
	}
	return (1);
}

int	ft_puthex(unsigned int nbr, char c)
{
	if (nbr == 0)
		return (ft_putchar_fd('0', STDOUT_FILENO));
	else
	{
		if (ft_printhex(nbr, c) == -1)
			return (-1);
		return (ft_length(nbr));
	}
}
