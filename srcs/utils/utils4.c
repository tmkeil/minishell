/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:56:46 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 17:35:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_send_to_ipc(char **envps, int ipc)
{
	int	i;

	i = 0;
	while (envps[i])
	{
		ft_putstr_fd(envps[i], ipc);
		ft_putstr_fd("\n", ipc);
		i++;
	}
}

void	ft_transfer_child_parent(t_envs **envs, int ipc)
{
	char	*line;
	char	**split;

	// printf("test1\n");
	while (1)
	{
		line = get_next_line(ipc);
		if (!line)
			break ;
		split = ft_split(line, '=');
		// printf("line = %s\n", line);
		// printf("before freeing line\n");
		free(line);
		// printf("after freeing line\n");
		if (!split)
			return ;
		ft_set_env(split[0], split[1], envs);
		ft_free_ptr(&split);
	}
	// printf("test2\n");
}
