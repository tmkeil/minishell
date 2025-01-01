/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:55:50 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/01 11:54:35 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strndup(const char *s, size_t n)
{
    char *dup;
    size_t i;

    dup = malloc(n + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < n && s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
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

int	ft_strappend(char **str, char c)
{
	size_t	len;
	char	*new_str;

	if (!str)
		return (0);
	len = ft_strlen(*str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (0);
	ft_memcpy(new_str, *str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(*str);
	*str = new_str;
	return (1);
}

int	ft_join(char **str, char *to_join)
{
	char	*tmp;

	if (!str || !to_join)
		return (0);
	tmp = *str;
	*str = ft_strjoin(*str, to_join);
	free(tmp);
	if (!*str)
		return (0);
	return (1);
}

char **ft_split_once(char *str, char delimiter) 
{
    char *pos;
    char **result;
    
    pos = ft_strchr(str, delimiter);
    result = (char **)malloc(3 * sizeof(char *));
    if (!result)
    {
        ft_printf("Memory allocation failed\n");
        exit(1);
    }
    if (!pos)
    {
        result[0] = ft_strdup(str);
        result[1] = NULL;
        return (result);
    }
    result[0] = ft_strndup(str, pos - str);
    result[1] = ft_strdup(pos + 1);
    result[2] = NULL;
    return (result);
}
