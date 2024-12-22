/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:21:34 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/22 16:02:28 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_env(t_lexems *lexems, t_envs *envs)
{
	if (!lexems || ft_strncmp(lexems->value, "env", 3) != 0)
		return (0);
    return (ft_print_envs(envs));
}
