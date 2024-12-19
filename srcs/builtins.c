/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/19 15:40:09 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_name(char *name)
{
	int	i;

	i = 0;
	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

void	update_env_var(const char *name, const char *value,
		t_env_node **envp_list)
{
	t_env_node	*current;
	t_env_node	*new_node;

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
	new_node = malloc(sizeof(t_env_node));
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

int	ft_handle_export(t_lexems *lexems, t_env_node *envp_list)
{
	t_lexems	*current;
	char		**values_to_set_env;
	t_env_node	*current_envp;

	if (ft_strncmp(lexems->value, "export", 7) == 0)
	{
		if (!lexems || !lexems->next || !ft_strchr(lexems->next->value, '='))
		{
			current_envp = envp_list;
			while (current_envp)
			{
				if (current_envp->value)
				{
					ft_printf("%s=%s\n", current_envp->name,
						current_envp->value);
				}
				else
				{
					ft_printf("%s\n", current_envp->name);
				}
				current_envp = current_envp->next;
			}
		}
		current = lexems->next;
		while (current && ft_strchr(current->value, '='))
		{
			values_to_set_env = ft_split(current->value, '=');
			if (values_to_set_env[0] && is_valid_env_name(values_to_set_env[0]))
				update_env_var(values_to_set_env[0], values_to_set_env[1],
					&envp_list);
			else
				ft_printf("export: `%s': not a valid identifier\n",
					current->value);
			clean_args(values_to_set_env);
			current = current->next;
		}
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

int	ft_unset(t_lexems *lexems, t_env_node *envp_list)
{
	(void)lexems;
	(void)envp_list;

	if (ft_strncmp(lexems->value, "unset", 6) == 0)
	{
		if (!lexems || !lexems->next)
			ft_printf("unset: not enough arguments\n");
		return (1);
	}
	return (0);
}
