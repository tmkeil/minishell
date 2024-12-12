/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:05:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:05:01 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	char	*s;
	int		count;

	s = ft_itoa(n);
	if (!s)
		return (-1);
	count = ft_putstr_fd(s, fd);
	return (free(s), count);
}

// void	ft_putnbr_fd(int n, int fd)
// {
// 	char	*ptr;
// 	ptr = ft_itoa(n);
// 	if (!ptr)
// 		return ;
// 	ft_putstr_fd(ptr, fd);
// 	free(ptr);
// }
//  int main(void)
// {
//      ft_putnbr_fd(2147483647, 1);
//      return (0);
//  }