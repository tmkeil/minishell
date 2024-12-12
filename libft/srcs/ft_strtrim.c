/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:11:26 by tkeil             #+#    #+#             */
/*   Updated: 2024/10/08 13:11:26 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_occur(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	size_t	pre;
	size_t	post;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	pre = 0;
	post = ft_strlen(s1) - 1;
	while (s1[pre] && ft_occur(set, s1[pre]))
		pre++;
	while (post > pre && ft_occur(set, s1[post]))
		post--;
	ptr = ft_substr(s1, pre, post - pre + 1);
	return (ptr);
}

//  int main(void)
//  {
//      char *s1 = "aaaa";
//      char *set;
//     printf("trimmed str: %s\n", ft_strtrim(s1, set));
//      return (0);
//  }