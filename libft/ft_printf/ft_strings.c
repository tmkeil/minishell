/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strings.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 19:45:20 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/19 19:45:20 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (ft_putchar_fd(c, STDOUT_FILENO));
}

int	ft_putstr(char *s)
{
	return (ft_putstr_fd(s, STDOUT_FILENO));
}
