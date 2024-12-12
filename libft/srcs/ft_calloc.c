/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:31:59 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 12:31:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nelem, size_t elsize)
{
	void	*ptr;

	if (!nelem || !elsize)
	{
		ptr = malloc(0);
		if (!ptr)
			return (NULL);
		ft_bzero(ptr, 1);
		return (ptr);
	}
	ptr = malloc(nelem * elsize);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nelem * elsize);
	return (ptr);
}

// #include <stdio.h>
// int main(void)
// {
//  	void *str = calloc(0, 0);
// 	if (calloc(0, 0) == 0)
// 	{
// 		printf("real calloc: (null)\n");
// 	}
// 	printf("my calloc: %s\n", ft_calloc(0, 0));
//  	if (str == ((void *)0))
// 	{
// 		printf("0\n");
// 		exit(0);
// 	}
//  	free(str);
// 	printf("1\n");
//  	exit(1);
// }