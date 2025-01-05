/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:46:35 by frocha            #+#    #+#             */
/*   Updated: 2025/01/05 13:45:42 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_next_iteration(t_minishell **minishell)
{
	ft_free_tokens(&(*minishell)->tokens);
	ft_free_table(&(*minishell)->table);
	ft_free_cmds(&(*minishell)->cmds);
}

void	ft_free_shell(t_minishell **minishell)
{
	ft_free_tokens(&(*minishell)->tokens);
	ft_free_table(&(*minishell)->table);
	ft_free_envs(&(*minishell)->envs);
	ft_free_cmds(&(*minishell)->cmds);
	ft_free_ptr(&(*minishell)->envps);
}
