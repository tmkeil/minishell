/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_environments.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:48:18 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/17 19:49:46 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t find_prefix_length(const char *ptr)
{
    size_t i = 0;
    while (ptr[i] && ptr[i] != '$')
        i++;
    return i;
}

char *get_prefix(const char *ptr, size_t length)
{
    char *prefix = malloc(length + 1);
    if (!prefix)
        return NULL;
    ft_strlcpy(prefix, ptr, length + 1);
    return prefix;
}

int is_valid_env_char(char c)
{
    return ft_isalnum(c) || c == '_';
}

char *handle_lone_dollar(char *prefix, size_t *counter, size_t length)
{
    char *result = ft_strjoin(prefix, "$");
    free(prefix);
    *counter += length + 1;
    return result;
}

char *get_env_name(const char *ptr, size_t start, size_t *end)
{
    size_t j = start;
    while (ptr[j] && is_valid_env_char(ptr[j]))
        j++;
    size_t var_len = j - start;
    char *var_name = malloc(var_len + 1);
    if (!var_name)
        return NULL;
    ft_strlcpy(var_name, ptr + start, var_len + 1);
    *end = j;
    return var_name;
}

char *replace_with_env_value(char *prefix, const char *env_name)
{
    char *env_value = getenv(env_name);
    if (!env_value)
        env_value = "";
    char *result = ft_strjoin(prefix, env_value);
    free(prefix);
    return result;
}

char *ft_until_next_env(char *ptr, size_t *counter)
{
    size_t i = find_prefix_length(ptr);
    char *prefix = get_prefix(ptr, i);
    if (!prefix)
        return NULL;

    if (ptr[i] != '$')
    {
        *counter += i;
        return prefix;
    }
    if (!ptr[i + 1] || !is_valid_env_char(ptr[i + 1]))
        return handle_lone_dollar(prefix, counter, i);
    size_t j;
    char *env_name = get_env_name(ptr, i + 1, &j);
    if (!env_name)
    {
        free(prefix);
        return NULL;
    }
    char *result = replace_with_env_value(prefix, env_name);
    free(env_name);
    *counter += j;
    return result;
}

char *handle_environments(char *str)
{
    char *final_str = ft_strdup("");
    char *temp;
    char *next_part;
    size_t counter = 0;

    if (!final_str)
        return NULL;
    while (str[counter])
    {
        next_part = ft_until_next_env(str + counter, &counter);
        if (!next_part)
        {
            free(final_str);
            return NULL;
        }
        temp = final_str;
        final_str = ft_strjoin(final_str, next_part);
        free(temp);
        free(next_part);
    }
    return final_str;
}
