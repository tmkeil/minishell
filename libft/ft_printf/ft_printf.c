/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:40:59 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/25 15:40:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_specifier(const char *c, va_list arg)
{
	if (*c == 'c')
		return (ft_putchar(va_arg(arg, int)));
	else if (*c == 's')
		return (ft_putstr(va_arg(arg, char *)));
	else if (*c == 'p')
		return (ft_putptr(va_arg(arg, void *)));
	else if (*c == 'd' || *c == 'i')
		return (ft_putnbr(va_arg(arg, int)));
	else if (*c == 'u')
		return (ft_putunbr(va_arg(arg, unsigned int)));
	else if (*c == 'x' || *c == 'X')
		return (ft_puthex(va_arg(arg, unsigned int), *c));
	else if (*c == '%')
		return (ft_putchar('%'));
	return (0);
}

static int	ft_updateret_spec(int *ret, const char *format, va_list arg)
{
	int	tmp;

	tmp = ft_specifier(format, arg);
	if (tmp == -1)
		return (-1);
	return (*ret += tmp, *ret);
}

static int	ft_updateret(int *ret, const char c)
{
	if (ft_putchar(c) == -1)
		return (-1);
	return (*ret += 1, *ret);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		ret;

	va_start(args, format);
	ret = 0;
	while (*format)
	{
		if (*format == '%')
		{
			if (ft_updateret_spec(&ret, ++format, args) == -1)
				return (-1);
		}
		else
		{
			if (ft_updateret(&ret, *format) == -1)
				return (-1);
		}
		format++;
	}
	va_end(args);
	return (ret);
}

// #include <stdio.h>
// int main(void)
// {
// 	ft_printf("%i\n", 5);
//     return (0);
// }