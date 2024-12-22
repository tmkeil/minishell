/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:05:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/22 13:25:22 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_exit(t_minishell **minishell, t_lexems *lexems, int index)
{
	if (index == 0 && ft_strncmp(lexems->value, "exit", 4) == 0)
	{
		ft_free_shell(minishell);
		// system("leaks minishell");
		exit((*minishell)->exit_status);
	}
	return (0);
}
