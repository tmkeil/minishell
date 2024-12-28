/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:04:42 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 10:56:37 by tkeil            ###   ########.fr       */
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

int	ft_unset_key(char *key_to_unset, t_envs **envs)
{
	t_envs	*current;
	t_envs	*last;

	current = *envs;
	last = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, key_to_unset, !ft_strlen(key_to_unset)
				+ 1) == 0)
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

void	ft_process_unset_key(t_minishell **minishell, char *key, t_envs **envs, int *status, int *n)
{
	if (ft_strncmp(key, " ", 1) == 0)
		ft_putendl_fd("unset: not enough arguments", STDERR_FILENO);
	else
	{
		++(*n);
		if (!ft_is_valid_identifier(key))
		{
			ft_putstr_fd("unset:", STDERR_FILENO);
			ft_putstr_fd(key, STDERR_FILENO);
			ft_putendl_fd(": invalid parameter name", STDERR_FILENO);
			(*minishell)->exit_status = 1;
			*status = 2;
		}
		else
			ft_unset_key(key, envs);
	}
}

int	ft_unset(t_minishell **minishell, t_lexems *lexems, t_envs **envs)
{
	int	status;
	int	count;

	status = 1;
	count = 0;
	if (!lexems->next)
	{
		(*minishell)->exit_status = 1;
		ft_putendl_fd("unset: not enough arguments", STDERR_FILENO);
		return (2);
	}
	lexems = lexems->next->next;
	while (lexems)
	{
		if (ft_strncmp(lexems->value, " ", 1) == 0)
			lexems = lexems->next;
		ft_process_unset_key(minishell, lexems->value, envs, &status, &count);
		lexems = lexems->next;
	}
	if (!count)
		return (ft_putendl_fd("unset: not enough arguments", STDERR_FILENO), 2);
	return (status);
}
