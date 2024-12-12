/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 12:58:15 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 12:58:15 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}

// #include <stdio.h>
// int main(void)
// {
// 	t_list *t;
// 	t = ft_lstnew("node 1");
// 	ft_lstadd_back(&t, ft_lstnew("node 2"));
// 	while (t)
// 	{
// 		printf("%s\n", t->content);
// 		t = t->next;
// 	}
// 	return (0);
// }