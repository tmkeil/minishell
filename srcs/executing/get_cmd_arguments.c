/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/21 00:00:38 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_append_args(char **args, int i, char *value)
{
	char	*tmp;

	tmp = args[i];
	args[i] = ft_strjoin(args[i], value);
	free(tmp);
}

char	*ft_get_env(char *env_var, t_envs *envs)
{
	t_envs	*current;
	size_t	env_var_size;

	env_var_size = ft_strlen(env_var);
	current = envs;
	while (current)
	{
		if (ft_strncmp(current->name, env_var, env_var_size + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	ft_handle_env(char **args, int i, char **current, t_envs *envs)
{
	char	*ptr;
	size_t	len;
	char	*env_var;
	char	*value;

	ptr = *current + 1;
	if (*ptr == '$' || (!ft_isalpha(*ptr) && !ft_strchr("_?", *ptr)))
	{
		ft_append_args(args, i, "$");
		*current += 1 * (*ptr == '$') + (*ptr != '$');
		return ;
	}
	len = ft_find_end(ptr) - ptr;
	env_var = ft_substr(ptr, 0, len);
	value = ft_get_env(env_var, envs);
	if (value)
		ft_append_args(args, i, value);
	else
		ft_append_args(args, i, "");
	free(env_var);
	*current += len + 1;
}

void	ft_handle_lexem(char **args, int i, char *current, t_types type,
		t_envs *envs)
{
	size_t	len;
	char	*sub;

	while (*current)
	{
		if (type == SINGLE_QUOTE)
		{
			sub = ft_substr(current, 0, ft_strlen((char *)current));
			ft_append_args(args, i, sub);
			free(sub);
			return ;
		}
		if (*current != '$')
		{
			len = ft_until_next_env(current) - current;
			sub = ft_substr(current, 0, len);
			ft_append_args(args, i, sub);
			free(sub);
			current += len;
		}
		else
			ft_handle_env(args, i, &current, envs);
	}
}
