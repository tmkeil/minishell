/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:08:39 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:08:39 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_length;

	i = 0;
	dst_len = ft_strlen(dst);
	src_length = ft_strlen(src);
	if (!dst || !src)
		return (0);
	if (size == 0)
		return (src_length);
	if (size <= dst_len)
		return (size + src_length);
	while (src[i] && (size - dst_len - 1) > i)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_length);
}
