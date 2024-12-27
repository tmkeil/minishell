/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:05:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/27 13:47:49 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_exit(t_minishell **minishell)
{
	ft_free_shell(minishell);
	// system("leaks minishell");
	exit((*minishell)->exit_status);
}
