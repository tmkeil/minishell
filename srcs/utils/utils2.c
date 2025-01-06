/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:25:02 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 15:14:02 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_envs(t_envs *envs)
{
	while (envs)
	{
		ft_putstr_fd(envs->name, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putstr_fd(envs->value, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		envs = envs->next;
	}
}

int	ft_split_env(char *env_var, char **name, char **value)
{
	char	**split;

	split = ft_split(env_var, '=');
	if (!split)
		return (0);
	*name = ft_strdup(split[0]);
	*value = ft_strdup(split[1]);
	return (ft_free_ptr(&split), 1);
}

int	ft_extract_envps(t_envs **envs, char **envp)
{
	t_envs	*current;
	t_envs	*new_node;
	int		i;

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
	return (ft_set_env("?", "0", envs));
}

static int	ft_prepare_envs(t_envs *envs, char ***envps)
{
	size_t	size;

	size = 0;
	ft_free_ptr(envps);
	while (envs)
	{
		envs = envs->next;
		size++;
	}
	*envps = malloc(sizeof(char *) * (size + 1));
	if (!*envps)
		return (0);
	while (size)
	{
		(*envps)[size] = NULL;
		size--;
	}
	return (1);
}

int	ft_update_envps(t_envs *envs, char ***envps)
{
	size_t	i;
	char	*tmp;
	t_envs	*ptr;

	i = 0;
	ptr = envs;
	if (!ft_prepare_envs(ptr, envps))
		return (0);
	ptr = envs;
	while (ptr)
	{
		tmp = ft_strjoin(ptr->name, "=");
		if (!tmp)
			return (ft_free_ptr(envps), 0);
		(*envps)[i] = ft_strjoin(tmp, ptr->value);
		free(tmp);
		if (!(*envps)[i])
			return (ft_free_ptr(envps), 0);
		ptr = ptr->next;
		i++;
	}
	return (1);
}
