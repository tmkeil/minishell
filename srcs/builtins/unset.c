/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 21:05:01 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_lexems *lexems, t_envs **envp_list)
{
	t_envs	*previous;
	t_envs	*current;
	char	*key_to_unset;

	if (ft_strncmp(lexems->value, "unset", 6) == 0)
	{
		if (!lexems || !lexems->next)
		{
			ft_printf("unset: not enough arguments\n");
			return (1);
		}
		lexems = lexems->next;
		while (lexems)
		{
			key_to_unset = lexems->value;
			current = *envp_list;
			previous = NULL;
			while (current)
			{
				if (ft_strncmp(current->name, key_to_unset,
						ft_strlen(key_to_unset) + 1) == 0)
				{
					if (previous)
						previous->next = current->next;
					else
						*envp_list = current->next;
					free(current->name);
					free(current->value);
					free(current);
					break ;
				}
				previous = current;
				current = current->next;
			}
			lexems = lexems->next;
		}
		return (1);
	}
	return (0);
}
