/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/04 20:33:56 by tkeil            ###   ########.fr       */
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

int	ft_unset(t_minishell **minishell, char **args, t_envs **envs, char ***envps)
{
	int	i;

	i = 1;
	while (args[i])
	{
		ft_unset_key(args[i], envs);
		i++;
	}
	i = 1;
	while (args[i])
	{
		if (!ft_is_valid_identifier(args[i]))
		{
			ft_put_error_str("bash: unset: `", NULL);
			ft_put_error_str(args[i], "': not a valid identifier");
			(*minishell)->exit_status = 1;
			break ;
		}
		i++;
	}
	ft_update_envps(*envs, envps);
	return (1);
}
