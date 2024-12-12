/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:00:59 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:00:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*tmp;
	void	*ret;

	if (!lst || !f || !del)
		return (NULL);
	new = NULL;
	while (lst)
	{
		ret = (*f)(lst->content);
		tmp = ft_lstnew(ret);
		if (!tmp)
		{
			(del)(ret);
			ft_lstclear(&new, del);
			return (NULL);
		}
		ft_lstadd_back(&new, tmp);
		lst = lst->next;
	}
	return (new);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// void del(void *s)
// {
//     free(s);
// }
// void *str_to_upper(void *s)
// {
//     char *str = strdup((char *)s);
// 	int	i;
// 	i = 0;
// 	if (!str)
// 		return (NULL);
// 	while (str[i])
// 	{
//         str[i] = str[i] - 32;
// 		i++;
//     }
//     return str;
// }
// void print_list(t_list *list)
// {
// 	if (!list)
// 	{
// 		return ;
// 	}
//     while (list)
// 	{
//         printf("%s\n", (char *)list->content);
//         list = list->next;
//     }
// }
// int main(void)
// {
// 	t_list *t;
// 	t_list *map;
// 	t = ft_lstnew("nodea");
// 	ft_lstadd_back(&t, ft_lstnew(strdup("nodeb")));
//     ft_lstadd_back(&t, ft_lstnew(strdup("nodec")));
// 	printf("Original list:\n");
//     print_list(t);
// 	printf("\n");
// 	map = ft_lstmap(t, str_to_upper, del);
// 	printf("Mapped:\n");
//     print_list(map);
// 	ft_lstclear(&t, del);
// 	ft_lstclear(&map, del);
// 	printf("After clearing:\n");
//     print_list(map);
// 	print_list(t);
// 	return (0);
// }