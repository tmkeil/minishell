/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:22:30 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/04 17:22:30 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(int val)
{
	int	i;

	i = 0;
	if (val <= 0)
	{
		i++;
		val = -val;
	}
	while (val)
	{
		val /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int val)
{
	int		len;
	char	*s;
	int		sign;

	if (val == INT_MIN)
		return (ft_strdup("-2147483648"));
	sign = 0;
	len = ft_len(val);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	if (val < 0)
	{
		sign = 1;
		val = -val;
		s[0] = '-';
	}
	s[len] = '\0';
	while (len - sign > 0)
	{
		s[--len] = val % 10 + '0';
		val /= 10;
	}
	return (s);
}

// #include <stdio.h>
// int main(void)
// {
//     int zahl = INT_MIN;
//     printf("zahl = %s\n", ft_itoa(zahl));
//     return (0);
// }