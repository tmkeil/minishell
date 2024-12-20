/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:46:35 by frocha            #+#    #+#             */
/*   Updated: 2024/12/20 21:19:33 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_ptr(char ***ptr)
{
	int	i;

	if (!ptr || !*ptr || !**ptr)
		return ;
	i = 0;
	while ((*ptr)[i])
	{
		free((*ptr)[i]);
		i++;
	}
	free(*ptr);
	*ptr = NULL;
}

void	ft_free_tokens(t_lexems **lexems)
{
	t_lexems	*tmp;

	if (!lexems || !(*lexems))
		return ;
	while (*lexems)
	{
		tmp = (*lexems)->next;
		free((*lexems)->value);
		free(*lexems);
		*lexems = tmp;
	}
	*lexems = NULL;
}

void	ft_free_table(t_lexems ***table)
{
	int			i;
	t_lexems	*current;
	t_lexems	*next;

	if (!table || !*table)
		return ;
	i = 0;
	while ((*table)[i])
	{
		current = (*table)[i];
		while (current)
		{
			next = current->next;
			if (current->value)
			{
				free(current->value);
				current->value = NULL;
			}
			free(current);
			current = next;
		}
		i++;
	}
	free(*table);
	*table = NULL;
}

void	ft_free_envs(t_envs **head)
{
	t_envs	*current;

	while (*head)
	{
		current = *head;
		*head = (*head)->next;
		free(current->name);
		free(current->value);
		free(current);
	}
}

void	ft_free_minishell(t_minishell **minishell)
{
	ft_free_tokens(&(*minishell)->tokens);
	ft_free_table(&(*minishell)->table);
	ft_free_envs(&(*minishell)->envs);
}
