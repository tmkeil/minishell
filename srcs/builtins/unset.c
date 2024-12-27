/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/27 14:32:48 by tkeil            ###   ########.fr       */
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

void ft_process_unset_key(char *key, t_envs **envs, int *returned_elem)
{
    if (*key != '\0' && !ft_is_valid_identifier(key))
	{
		*returned_elem = 0;
        ft_printf("unset: `%s`: not a valid identifier\n", key);
	}
    else
        ft_unset_key(key, envs);
}

int	ft_unset(t_lexems *lexems, t_envs **envs)
{
	int returned_element;

	returned_element = 1;
	if (!lexems || ft_strncmp(lexems->value, "unset", 6) != 0)
		return (0);
	if (!lexems->next)
	{
		ft_printf("unset: not enough arguments\n");
		return (0);
	}
	lexems = lexems->next;
	while (lexems)
	{
		ft_process_unset_key(lexems->value, envs, &returned_element);
		lexems = lexems->next;
	}
	if (returned_element == 0)
	{
		exit(EXIT_FAILURE);
	}
	return (returned_element);
}
