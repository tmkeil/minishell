/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:59:17 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 12:59:17 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;

	if (!lst || !*lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
	free(*lst);
	*lst = NULL;
}

// #include <stdio.h>
// void	del(void *s)
// {
// }
// int main(void)
// {
// 	t_list *t;
// 	t_list *tmp;
// 	t_list *tmp2;
// 	t = ft_lstnew("node 1");
// 	ft_lstadd_back(&t, ft_lstnew("node 2"));
// 	ft_lstadd_back(&t, ft_lstnew("node 3"));
// 	tmp = t;
// 	while (tmp)
// 	{
// 		printf("%s\n", tmp->content);
// 		tmp = tmp->next;
// 	}
// 	printf("\n");
// 	ft_lstclear(&t, del);
// 	tmp2 = t;
// 	while (tmp2)
// 	{
// 		printf("%s\n", tmp2->content);
// 		tmp2 = tmp2->next;
// 	}
// 	ft_lstclear(&t, del);
// }