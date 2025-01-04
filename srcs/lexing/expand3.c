/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:02:57 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/04 19:03:08 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_find_end(char *ptr)
{
	int	i;

	i = 0;
	if (ptr[i] == '?')
		return (&ptr[i + 1]);
	while (ptr[i] && (ft_isalnum(ptr[i]) || ft_strchr("_", ptr[i])))
		i++;
	return (&ptr[i]);
}

char	*ft_until_next_env(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && ptr[i] != '$')
		i++;
	return (&ptr[i]);
}
