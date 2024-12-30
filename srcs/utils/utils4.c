/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frocha <frocha@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 19:02:23 by frocha            #+#    #+#             */
/*   Updated: 2024/12/30 19:02:27 by frocha           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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