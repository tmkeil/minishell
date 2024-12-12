/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:01:44 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:01:44 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

// int main(void)
// {
//     t_list *list = NULL;
//     t_list *node1 = ft_lstnew("Node1");
//     t_list *node2 = ft_lstnew("Node2");
//     ft_lstadd_front(&list, node2);
//     ft_lstadd_front(&list, node1);
//     printf("length = %i\n", ft_lst_size(list));
//     return (0);
// }