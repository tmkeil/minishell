/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:10:15 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/27 13:43:34 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(void)
{
    char    buffer[1024];

    if (!getcwd(buffer, sizeof(buffer)))
        return (perror("getcwd"), 0);
    return (ft_putendl_fd(buffer, STDOUT_FILENO), 1);
}
