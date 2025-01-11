/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 14:16:11 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 22:52:13 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_append_new_command(t_cmds **cmds, t_cmds *new)
{
	t_cmds	*last;

	if (!new)
		return ;
	if (!*cmds)
	{
		*cmds = new;
		return ;
	}
	last = *cmds;
	while (last->next)
		last = last->next;
	last->next = new;
}

void	ft_init_new(t_cmds **cmd)
{
	(*cmd)->cmd = NULL;
	(*cmd)->args = NULL;
	(*cmd)->input_file = NULL;
	(*cmd)->output_file = NULL;
	(*cmd)->append = 0;
	(*cmd)->heredoc = NULL;
	(*cmd)->next = NULL;
}

int	ft_alloc_args(t_cmds **cmd, t_lexems *lexem)
{
	int	size;

	size = 0;
	while (lexem)
	{
		if (ft_strnstr(OPERATIONS, (char *)lexem->value,
				ft_strlen(OPERATIONS)))
			break ;
		if (lexem->type == WORD)
			size++;
		lexem = lexem->next;
	}
	(*cmd)->args = malloc(sizeof(char *) * (size + 1));
	if (!(*cmd)->args)
		return (0);
	((*cmd)->args)[size] = NULL;
	return (1);
}

int	ft_fill_args(t_cmds **cmd, t_lexems **lexem)
{
	int	i;

	i = 0;
	while (*lexem)
	{
		if (ft_strnstr(OPERATIONS, (char *)(*lexem)->value,
			ft_strlen(OPERATIONS)))
			break ;
		((*cmd)->args)[i] = ft_strdup((char *)(*lexem)->value);
		if (!((*cmd)->args)[i])
			return (0);
		i++;
		*lexem = (*lexem)->next;
	}
	return (1);
}
