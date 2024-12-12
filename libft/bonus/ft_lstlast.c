/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:00:39 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:00:39 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

// #include <stdio.h>
//  int main(void)
//  {
//      t_list *list = NULL;
//      t_list *node1 = ft_lstnew("Node1");
//      t_list *node2 = ft_lstnew("Node2");
//      ft_lstadd_front(&list, node2);
//      ft_lstadd_front(&list, node1);
//      printf("last: %s\n", ft_lstlast(list)->content);
//      return (0);
//  }
// #include <stdio.h>
// int main(void)
// {
// 	 	t_list *l = ((void *)0);
//  	t_list *expected;
//  	t_list *actual;
//  	expected = ((void *)0);
//  	actual = ft_lstlast(l);
//  	if (actual == expected)
// 	{
// 		printf("a\n");
// 		exit(0);
// 	}
//  	printf("b\n");
// 	exit(1);
// }