/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:20:47 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 16:18:51 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_valid_env(const char *name)
{
	int	i;

	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_add_env(const char *name, const char *value, t_envs **envs)
{
	t_envs	*current;
	t_envs	*new_node;

	new_node = malloc(sizeof(t_envs));
	if (!new_node)
		return (0);
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	current = *envs;
	if (!current)
	{
		*envs = new_node;
		return (1);
	}
	while (current && current->next)
		current = current->next;
	current->next = new_node;
	return (1);
}

int	ft_set_env(const char *name, const char *value, t_envs **envs)
{
	t_envs	*current;

	current = *envs;
	while (current)
	{
		if (!ft_strncmp(current->name, name, ft_strlen(name) + 1))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (1);
		}
		current = current->next;
	}
	return (ft_add_env(name, value, envs));
}

int	ft_print_envs(t_envs *envs)
{
	while (envs)
	{
		if (envs->value)
			ft_printf("%s=%s\n", envs->name, envs->value);
		else
			ft_printf("%s\n", envs->name);
		envs = envs->next;
	}
	return (1);
}

int	ft_export(t_lexems *lexems, t_envs **envs, char ***envps)
{
	char	**env_args;

	if (!lexems->next || !lexems->next->next)
		return (ft_print_envs(*envs), 1);
	lexems = lexems->next->next;
	while (lexems && ft_strchr(lexems->value, '='))
	{
		env_args = ft_split(lexems->value, '=');
		if (ft_valid_env(env_args[0]))
		{
			if (!ft_set_env(env_args[0], env_args[1], envs))
				return (ft_free_ptr(&env_args), 2);
		}
		else
		{
			ft_putstr_fd("export: `", STDERR_FILENO);
			ft_putstr_fd((char *)lexems->value, STDERR_FILENO);
			ft_putendl_fd("\': not a valid identifier", STDERR_FILENO);
			return (2);
		}
		ft_free_ptr(&env_args);
		lexems = lexems->next;
	}
	return (ft_update_envps(*envs, envps));
}
