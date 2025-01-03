/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 23:19:25 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_remove_env(t_envs **envs, t_envs *current, t_envs *last)
{
	if (last)
		last->next = current->next;
	else
		*envs = current->next;
	free(current->name);
	free(current->value);
	free(current);
}

int	ft_unset_key(char *key, t_envs **envs)
{
	t_envs	*current;
	t_envs	*last;

	current = *envs;
	last = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, key, ft_strlen(key) + 1) == 0)
		{
			ft_remove_env(envs, current, last);
			break ;
		}
		last = current;
		current = current->next;
	}
	return (1);
}

int	ft_is_valid_identifier(const char *key)
{
	if (!key || !*key)
		return (0);
	if (!ft_isalpha(*key) && *key != '_')
		return (0);
	while (*key)
	{
		if (!ft_isalnum(*key) && *key != '_')
			return (0);
		key++;
	}
	return (1);
}

void	ft_process_unset_key(char *key, t_envs **envs, int *n)
{
	// if (ft_strncmp(key, " ", 1) == 0)
	// 	ft_putendl_fd("unset: not enough arguments", STDERR_FILENO);
	if (ft_strncmp(key, " ", 1) != 0)
	{
		++(*n);
		printf("key that should be unset: %s\n", key);
		if (!ft_is_valid_identifier(key))
		{
			ft_put_error_str("unset:", NULL);
			ft_put_error_str(key, ": invalid parameter name");
		}
		else
			ft_unset_key(key, envs);
	}
}

void	ft_unset(t_minishell **minishell, t_lexems *lexems, t_envs **envs, char ***envps, int ipc)
{
	int	count;

	count = 0;
	printf("in unset func\n");
	if (!lexems->next)
		exit(EXIT_SUCCESS);
	lexems = lexems->next->next;
	while (lexems)
	{
		if (lexems->type == SEPERATOR)
			lexems = lexems->next;
		ft_process_unset_key(lexems->value, envs, &count);
		lexems = lexems->next;
	}
	if (!count)
	{
		ft_putendl_fd("unset: not enough arguments", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_update_envps(*envs, envps);
	ft_send_to_ipc(minishell, *envps, ipc);
	
	exit(EXIT_SUCCESS);
}
