/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:59:54 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 12:59:54 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	(*del)(lst->content);
	free(lst);
}

// int main(void)
// {
//     t_list *list = NULL;
//     t_list *node1 = ft_lstnew("Node1");
//     t_list *node2 = ft_lstnew("Node2");
//     ft_lstadd_front(&list, node2);
//     ft_lstadd_front(&list, node1);
//     return (0);
// }