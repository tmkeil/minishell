/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:02:00 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:02:00 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	cc;

	ptr = (unsigned char *) s;
	cc = (unsigned char) c;
	while (n--)
	{
		if (*ptr == cc)
			return ((void *) ptr);
		ptr++;
	}
	return (NULL);
}
