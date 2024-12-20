/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/19 19:17:56 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_find_end(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && (ft_isalnum(ptr[i]) || ft_strchr("_?", ptr[i])))
		i++;
	return (&ptr[i]);
}

char	*ft_until_next_env(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && ptr[i] != '$')
		i++;
	return (&ptr[i]);
}

void	append_to_args(char **args, int i, char *value)
{
	char	*tmp;

	tmp = args[i];
	args[i] = ft_strjoin(args[i], value);
	free(tmp);
}

char	*get_value_env_linked_list(char *env_var, t_env_node *envp_list)
{
	t_env_node	*current;
	size_t		env_var_size;

	env_var_size = ft_strlen(env_var);
	current = envp_list;
	while (current)
	{
		if (ft_strncmp(current->name, env_var, env_var_size + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	handle_env_var(char **args, int i, char **current,
		t_env_node *envp_list)
{
	char	*ptr;
	size_t	len;
	char	*env_var;
	char	*value;

	ptr = *current + 1;
	if (*ptr == '$' || (!ft_isalpha(*ptr) && !ft_strchr("_?", *ptr)))
	{
		append_to_args(args, i, "$");
		*current += 1 * (*ptr == '$') + (*ptr != '$');
		return ;
	}
	len = ft_find_end(ptr) - ptr;
	env_var = ft_substr(ptr, 0, len);
	value = get_value_env_linked_list(env_var, envp_list);
	if (value)
		append_to_args(args, i, value);
	else
		append_to_args(args, i, "");
	free(env_var);
	*current += len + 1;
}

void	handle_lexem(char **args, int i, char *current, t_types type,
		t_env_node *envp_list)
{
	size_t	len;
	char	*sub;

	while (*current)
	{
		if (type == SINGLE_QUOTE)
		{
			sub = ft_substr(current, 0, ft_strlen((char *)current));
			append_to_args(args, i, sub);
			free(sub);
			return ;
		}
		if (*current != '$')
		{
			len = ft_until_next_env(current) - current;
			sub = ft_substr(current, 0, len);
			append_to_args(args, i, sub);
			free(sub);
			current += len;
		}
		else
			handle_env_var(args, i, &current, envp_list);
	}
}
