/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:11:05 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:11:05 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char			*ptr;
	unsigned char	cc;

	ptr = NULL;
	cc = (unsigned char) c;
	while (*s)
	{
		if (*s == cc)
			ptr = (char *) s;
		s++;
	}
	if (cc == '\0')
		return ((char *) s);
	return (ptr);
}
