/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 18:14:25 by tkeil            ###   ########.fr       */
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

int	ft_unset_key(char *key, t_envs **envs)
{
	t_envs	*current;
	t_envs	*last;

	current = *envs;
	last = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, key, ft_strlen(key) + 1) == 0)
		{
			ft_remove_env(envs, current, last);
			break ;
		}
		last = current;
		current = current->next;
	}
	return (1);
}

int	ft_is_valid_identifier(const char *key)
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

int	ft_unset(t_minishell **minishell, char **args, t_envs **envs)
{
	int		i;
	char	**name;

	i = 1;
	while (args[i])
	{
		ft_unset_key(args[i], envs);
		i++;
	}
	i = 1;
	ft_update_envps(*envs, &(*minishell)->envps);
	while (args[i])
	{
		name = ft_split(args[i], '=');
		if (!ft_is_valid_identifier(name[0]))
		{
			ft_sterr("bash: unset: `",
				args[i], "': not a valid identifier");
			return (ft_free_ptr(&name), EXIT_FAILURE);
		}
		ft_free_ptr(&name);
		i++;
	}
	return (EXIT_SUCCESS);
}
