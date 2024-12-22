/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:10:15 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/22 16:04:12 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(t_lexems *lexems)
{
    char    buffer[1024];

	if (!lexems || ft_strncmp(lexems->value, "pwd", 3) != 0)
		return (0);
    if (!getcwd(buffer, sizeof(buffer)))
        return (perror("getcwd"), 0);
    return (ft_putendl_fd(buffer, STDOUT_FILENO), 1);
}
