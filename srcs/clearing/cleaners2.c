/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:39:52 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 22:52:13 by tkeil            ###   ########.fr       */
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

void	ft_free_envs(t_envs **head)
{
	t_envs	*current;

	while (*head)
	{
		current = *head;
		*head = (*head)->next;
		free(current->name);
		free(current->value);
		current->name = NULL;
		current->value = NULL;
		free(current);
	}
	head = NULL;
}

void	ft_free_tokens(t_lexems **lexems)
{
	t_lexems	*tmp;

	if (!lexems || !(*lexems))
		return ;
	while (*lexems)
	{
		tmp = (*lexems)->next;
		if ((*lexems) && (*lexems)->value)
		{
			free((*lexems)->value);
			free(*lexems);
		}
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

void	ft_free_cmds(t_cmds **cmds)
{
	t_cmds	*current;

	if (!cmds || !*cmds)
		return ;
	while (*cmds)
	{
		current = *cmds;
		*cmds = (*cmds)->next;
		if (current->cmd)
			free(current->cmd);
		if (current->args)
			ft_free_ptr(&(current->args));
		if (current->input_file)
			free(current->input_file);
		if (current->output_file)
			free(current->output_file);
		if (current->heredoc)
			free(current->heredoc);
		free(current);
	}
	*cmds = NULL;
}
