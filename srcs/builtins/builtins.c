/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 15:34:56 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_name(char *name)
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

void	update_env_var(const char *name, const char *value, t_envs **envp_list)
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

int	ft_handle_export(t_lexems *lexems, t_envs **envp_list)
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
			if (env_args[0] && is_valid_env_name(env_args[0]))
				update_env_var(env_args[0], env_args[1], envp_list);
			else
				ft_printf("export: `%s': not a valid identifier\n",
					current->value);
			ft_clr(&env_args);
			current = current->next;
		}
		envp = *envp_list;
		return (1);
	}
	return (0);
}

int	ft_changedir(t_lexems *lexems)
{
	char	*home;

	home = NULL;
	if (lexems->type == WORD && !ft_strncmp((char *)lexems->value, "cd", 2))
	{
		if (lexems->next)
		{
			if (chdir((char *)lexems->next->value) != 0)
				printf("cd: no such file or directory: %s\n",
					(char *)lexems->next->value);
		}
		else
		{
			home = getenv("HOME");
			if (chdir(home) != 0)
				printf("cd: no such file or directory: %s\n", home);
		}
		return (1);
	}
	return (0);
}

int	ft_unset(t_lexems *lexems, t_envs **envp_list)
{
	t_envs	*previous;
	t_envs	*current;
	char	*key_to_unset;

	if (ft_strncmp(lexems->value, "unset", 6) == 0)
	{
		if (!lexems || !lexems->next)
		{
			ft_printf("unset: not enough arguments\n");
			return (1);
		}
		lexems = lexems->next;
		while (lexems)
		{
			key_to_unset = lexems->value;
			current = *envp_list;
			previous = NULL;
			while (current)
			{
				if (ft_strncmp(current->name, key_to_unset,
						ft_strlen(key_to_unset) + 1) == 0)
				{
					if (previous)
						previous->next = current->next;
					else
						*envp_list = current->next;
					free(current->name);
					free(current->value);
					free(current);
					break ;
				}
				previous = current;
				current = current->next;
			}
			lexems = lexems->next;
		}
		return (1);
	}
	return (0);
}
