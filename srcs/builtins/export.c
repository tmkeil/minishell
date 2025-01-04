/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:20:47 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/04 15:48:33 by tkeil            ###   ########.fr       */
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

	// printf("will be added: name = %s, value = %s\n", name, value);
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

void	ft_process_args(char *value, t_envs **envs)
{
	char	**env_args;

	env_args = ft_split_once(value, '=');
	if (ft_valid_env(env_args[0]) && ft_strchr(value, '='))
	{
		if (!ft_set_env(env_args[0], env_args[1], envs))
			ft_free_ptr(&env_args);
	}
	else if (!ft_valid_env(env_args[0]))
		ft_put_error_str(ERR_EXPORT, value);
	ft_free_ptr(&env_args);
}

void	ft_export(t_minishell **minishell, char **args, t_envs **envs, char ***envps)
{
	(void)minishell;
	(void)args;
	(void)envps;
	(void)envs;
	// if (!lexems->next || !lexems->next->next)
	// 	ft_print_envs(*envs);
	// lexems = lexems->next->next;
	// while (lexems)
	// {
	// 	if (lexems->type == SEPERATOR)
	// 		lexems = lexems->next;
	// 	if (!lexems)
	// 		break ;
	// 	ft_process_args((char *)lexems->value, envs);
	// 	lexems = lexems->next;
	// }
	// ft_update_envps(*envs, envps);
	// exit(EXIT_SUCCESS);
}
