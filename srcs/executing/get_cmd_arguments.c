/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 18:27:05 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_append_args(char **args, char *value)
{
	char	*tmp;

	tmp = *args;
	*args = ft_strjoin(*args, value);
	free(tmp);
}

void	ft_handle_lexem(char **args, char *current)
{
	size_t	len;
	char	*sub;

	while (*current)
	{
		len = ft_until_next_env(current) - current;
		sub = ft_substr(current, 0, len);
		ft_append_args(args, sub);
		free(sub);
		current += len;
	}
}
