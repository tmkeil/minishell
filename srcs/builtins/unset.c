/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 09:19:56 by tkeil            ###   ########.fr       */
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

int	ft_unset_key(char *key_to_unset, t_envs **envs)
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
	return (1);
}

int ft_is_valid_identifier(const char *key)
{
    if (!key || !*key)
        return (0);
    if (!ft_isalpha(*key) && *key != '_')
        return (0);
    while (*key)
    {
        if (!ft_isalnum(*key) && *key != '_')
            return (0);
        key++;
    }
    return (1);
}

int ft_process_unset_key(char *key, t_envs **envs)
{
    if (!ft_is_valid_identifier(key))
		return (0);
    else
        return (ft_unset_key(key, envs));
}

int	ft_unset(t_lexems *lexems, t_envs **envs)
{
	if (!lexems->next)
		return (0);
	lexems = lexems->next->next;
	while (lexems)
	{
		if (ft_strncmp(lexems->value, " ", 1) == 0)
			lexems = lexems->next;
		if (!ft_process_unset_key(lexems->value, envs))
			break ;
		lexems = lexems->next;
	}
	return (1);
}
