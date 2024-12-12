/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:03:43 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:03:43 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	unsigned char	cc;

	cc = (unsigned char) c;
	ptr = (unsigned char *) b;
	while (len--)
		*ptr++ = cc;
	return (b);
}

// #include <stdio.h>
// #include <string.h>
// int main() {
//     char array[10];
//     char arr2[10];
//     ft_memset(array, 'A', sizeof(array));
//     for (int i = 0; i < sizeof(array); i++) {
//         printf("%c ", array[i]);
//     }
//     printf("\n");
//     memset(arr2, 'c', sizeof(arr2));
//     for (int i = 0; i < sizeof(array); i++) {
//         printf("%c ", arr2[i]);
//     }
//     printf("\n");
//     return (0);
// }