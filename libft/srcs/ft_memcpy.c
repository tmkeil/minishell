/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:02:44 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:02:44 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*p1;
	const unsigned char	*p2;

	if (!src && !dest)
		return (NULL);
	p1 = (unsigned char *) dest;
	p2 = (unsigned char *) src;
	while (n--)
		*p1++ = *p2++;
	return (dest);
}
