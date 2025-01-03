/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 16:56:46 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 23:29:47 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**ft_dup_envps(char **envps)
// {
// 	int		i;
// 	int		count;
// 	char	**copy;

// 	i = 0;
// 	count = 0;
// 	while (envps[count])
// 		count++;
// 	copy = malloc(sizeof(char *) * (count + 1));
// 	if (!copy)
// 		return (NULL);
// 	while (i < count)
// 	{
// 		copy[i] = strdup(envps[i]);
// 		if (!copy[i])
// 		{
// 			ft_free_ptr(&copy);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	copy[count] = NULL;
// 	return (copy);
// }

void	ft_send_to_ipc(t_minishell **minishell, char **envps, int ipc)
{
	int	i;

	i = 0;
	(void)minishell;
	while (envps[i])
	{
		ft_putstr_fd(envps[i], ipc);
		ft_putstr_fd("\n", ipc);
		i++;
	}
}

void	ft_transfer_child_parent(t_minishell **minishell, t_envs **envs, int ipc, char ***envps)
{
	char	*line;
	char	**split;
	char	*trimmed;
	char	*trimmed2;

	// printf("inside transfer child1\n");
	(void)minishell;
	ft_free_envs(envs);
	// printf("after freeing envs again in transfer child\n");
	while (1)
	{
		line = get_next_line(ipc);
		if (!line)
			break ;
		// printf("line in transer child is: %s\n", line);
		printf("processing line: %s\n", line);
		split = ft_split(line, '=');
		free(line);
		if (!split)
			continue ;
		trimmed = ft_strtrim(split[1], "\n");
		trimmed2 = ft_strtrim(split[0], "\n");
		ft_set_env(trimmed2, trimmed, envs);
		free(trimmed);
		free(trimmed2);
		ft_free_ptr(&split);
	}
	// printf("inside transfer child11\n");
	(void)envps;
	ft_update_envps(*envs, envps);
	// printf("inside transfer child2\n");
}

// void	ft_transfer_child_parent(t_minishell **minishell, t_envs **envs, int ipc, char ***envps)
// {
// 	char	**split;
// 	char	*trimmed;
// 	char	*trimmed2;
// 	int		i;

// 	// printf("inside transfer child1\n");
// 	i = 0;
// 	(void)ipc;
// 	if ((*minishell)->cached_envps)
// 	{
// 		ft_free_envs(envs);
// 		// printf("after freeing envs again in transfer child\n");
// 		while (((*minishell)->cached_envps)[i])
// 		{
// 			// printf("line in transer child is: %s\n", line);
// 			split = ft_split(((*minishell)->cached_envps)[i], '=');
// 			if (!split)
// 				continue ;
// 			trimmed = ft_strtrim(split[1], "\n");
// 			trimmed2 = ft_strtrim(split[0], "\n");
// 			ft_set_env(trimmed2, trimmed, envs);
// 			free(trimmed);
// 			free(trimmed2);
// 			ft_free_ptr(&split);
// 		}
// 		// printf("inside transfer child11\n");
// 		(void)envps;
// 		// ft_update_envps(*envs, envps);
// 		// printf("inside transfer child2\n");
// 	}
// 	int k = 0;
// 	while ((*envps)[k])
// 	{
// 		printf("%s\n", (*envps)[k]);
// 		k++;
// 	}
// 	printf("in transf\n\n\n");
// 	ft_print_envs(*envs);
// }
