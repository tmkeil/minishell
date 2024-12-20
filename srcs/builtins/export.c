/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:20:47 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 21:18:12 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_valid_env(char *name)
{
	int	i;

	i = 0;
	if (!name || !(ft_isalpha(name[0]) || ft_strchr("_", name[0])))
		return (0);
	i = 1;
	while (name[i])
	{
		if ((!ft_isalnum(name[i]) && name[i] != '_'))
			return (0);
		i++;
	}
	return (1);
}

void	ft_set_env(const char *name, const char *value, t_envs **envp_list)
{
	t_envs	*current;
	t_envs	*new_node;

	current = *envp_list;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name) + 1) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new_node = malloc(sizeof(t_envs));
	if (!new_node)
		exit(EXIT_FAILURE);
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (current)
		current->next = new_node;
	else
		*envp_list = new_node;
}

int	ft_export(t_lexems *lexems, t_envs **envp_list)
{
	t_lexems	*current;
	char		**env_args;
	t_envs		*envp;

	if (ft_strncmp(lexems->value, "export", 7) == 0)
	{
		if (!lexems || !lexems->next || !ft_strchr(lexems->next->value, '='))
		{
			envp = *envp_list;
			while (envp)
			{
				if (envp->value)
					ft_printf("%s=%s\n", envp->name, envp->value);
				else
					ft_printf("%s\n", envp->name);
				envp = envp->next;
			}
			return (1);
		}
		current = lexems->next;
		while (current && ft_strchr(current->value, '='))
		{
			env_args = ft_split(current->value, '=');
			if (env_args[0] && ft_valid_env(env_args[0]))
				ft_set_env(env_args[0], env_args[1], envp_list);
			else
				ft_printf("export: `%s': not a valid identifier\n",
					current->value);
			ft_free_ptr(&env_args);
			current = current->next;
		}
		envp = *envp_list;
		return (1);
	}
	return (0);
}
