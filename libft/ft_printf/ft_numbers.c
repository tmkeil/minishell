/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numbers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:42:35 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/19 19:42:35 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	ft_length(unsigned int nbr)
{
	size_t	i;

	i = 0;
	while (nbr)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

static char	*ft_getunbr(unsigned int nbr)
{
	char	*n;
	size_t	length;

	length = ft_length(nbr);
	n = (char *)malloc(sizeof(char) * (length + 1));
	if (!n)
		return (NULL);
	n[length] = '\0';
	while (nbr)
	{
		n[length - 1] = nbr % 10 + '0';
		nbr /= 10;
		length--;
	}
	return (n);
}

int	ft_putunbr(unsigned int nbr)
{
	char	*n;
	int		count;

	count = 0;
	if (nbr == 0)
		return (ft_putchar_fd('0', STDOUT_FILENO));
	else
	{
		n = ft_getunbr(nbr);
		if (!n)
			return (-1);
		count = ft_putstr_fd(n, STDOUT_FILENO);
		return (free(n), count);
	}
}

int	ft_putnbr(int nbr)
{
	return (ft_putnbr_fd(nbr, STDOUT_FILENO));
}
