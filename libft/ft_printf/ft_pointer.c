/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:42:30 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/19 19:42:30 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_length(uintptr_t nbr)
{
	int	i;

	i = 0;
	while (nbr)
	{
		nbr /= 16;
		i++;
	}
	return (i);
}

static int	ft_printnbr(uintptr_t ptr)
{
	if (ptr >= 16)
	{
		if (ft_printnbr(ptr / 16) == -1)
			return (-1);
		if (ft_printnbr(ptr % 16) == -1)
			return (-1);
	}
	else
	{
		if (ft_putchar("0123456789abcdef"[ptr]) == -1)
			return (-1);
	}
	return (1);
}

int	ft_putptr(void *ptr)
{
	int	count;

	count = 2;
	if (ptr == NULL)
		return (ft_putstr("0x0"));
	if (ft_putstr("0x") == -1)
		return (-1);
	if (ft_printnbr((uintptr_t) ptr) == -1)
		return (-1);
	count += ft_length((uintptr_t) ptr);
	return (count);
}
