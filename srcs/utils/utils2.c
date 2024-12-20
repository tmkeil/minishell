/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:25:02 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 23:28:48 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_split_env(const char *env_var, char **name, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		*name = ft_substr(env_var, 0, equal_sign - env_var);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*name = ft_strdup(env_var);
		*value = NULL;
	}
	if (!*name || (equal_sign && !*value))
		return (0);
	return (1);
}

int	ft_extract_envps(t_envs **envs, char **envp)
{
	t_envs	*current;
	t_envs	*new_node;
	int		i;

	current = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_envs));
		if (!new_node)
			return (0);
		if (!ft_split_env(envp[i], &new_node->name, &new_node->value))
			return (free(new_node), 0);
		new_node->next = NULL;
		if (!*envs)
			*envs = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
	if (!ft_set_env("?", "0", envs))
		return (0);
	return (1);
}
