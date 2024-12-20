/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:05:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 22:55:02 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit(t_lexems *lexems, int index_lexem, t_minishell **minishell)
{
	if (index_lexem == 0 && ft_strncmp(lexems->value, "exit", 4) == 0)
	{
		ft_free_shell(minishell);
		// system("leaks minishell");
		exit((*minishell)->exit_status);
	}
}
