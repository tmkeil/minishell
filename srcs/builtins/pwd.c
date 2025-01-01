/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:10:15 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/01 19:45:21 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_pwd(void)
{
    char    buffer[1024];

    if (!getcwd(buffer, sizeof(buffer)))
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    ft_putendl_fd(buffer, STDOUT_FILENO);
    exit(EXIT_SUCCESS);
}
