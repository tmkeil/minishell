/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:25:02 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 18:05:14 by tkeil            ###   ########.fr       */
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

size_t	ft_prepare_envs(t_envs *envs, char ***envps)
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

void	ft_put_error_str(char *msg, char *value, int *exit_status, int *status)
{
	if (*status != -1)
	{
		*status = -1;
		*exit_status = 1;
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putendl_fd(value, STDERR_FILENO);
	}
	printf("status = %i\n", *status);
}
