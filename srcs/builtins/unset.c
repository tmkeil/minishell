/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/21 00:42:40 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_env(t_envs **envs, t_envs *current, t_envs *last)
{
	if (last)
		last->next = current->next;
	else
		*envs = current->next;
	free(current->name);
	free(current->value);
	free(current);
}

void	ft_unset_key(char *key_to_unset, t_envs **envs)
{
	t_envs	*current;
	t_envs	*last;

	current = *envs;
	last = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, key_to_unset, ft_strlen(key_to_unset) + 1) == 0)
		{
			ft_remove_env(envs, current, last);
			break ;
		}
		last = current;
		current = current->next;
	}
}

int	ft_unset(t_lexems *lexems, t_envs **envs)
{
	char	*key_to_unset;

	if (!lexems || ft_strncmp(lexems->value, "unset", 6) != 0)
		return (0);
	if (!lexems->next)
	{
		ft_printf("unset: not enough arguments\n");
		return (1);
	}
	lexems = lexems->next;
	while (lexems)
	{
		key_to_unset = lexems->value;
		ft_unset_key(key_to_unset, envs);
		lexems = lexems->next;
	}
	return (1);
}
