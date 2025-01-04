/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:58:06 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/04 18:55:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_append_new_command(t_cmds **cmds, t_cmds *new)
{
    t_cmds *last;

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

void    ft_init_new(t_cmds **cmd)
{
    (*cmd)->cmd = NULL;
    (*cmd)->args = NULL;
    (*cmd)->input_file = NULL;
    (*cmd)->output_file = NULL;
    (*cmd)->append = 0;
    (*cmd)->heredoc_end = NULL;
    (*cmd)->next = NULL;
}

int ft_alloc_args(t_cmds **cmd, t_lexems *lexem)
{
    int         size;
    
    size = 0;
    while (lexem)
    {
        if (ft_strnstr(OPERATIONS, (char *)lexem->value, ft_strlen(OPERATIONS)))
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

int ft_fill_args(t_cmds **cmd, t_lexems **lexem)
{
    int         i;
    
    i = 0;
	while (*lexem)
	{
		if (ft_strnstr(OPERATIONS, (char *)(*lexem)->value, ft_strlen(OPERATIONS)))
			break ;
        ((*cmd)->args)[i] = ft_strdup((char *)(*lexem)->value);
		if (!((*cmd)->args)[i])
			return (0);
		i++;
		*lexem = (*lexem)->next;
	}
	return (1);
}

int ft_get_new_cmd(t_cmds **cmd, t_lexems *lexem)
{
    while (lexem)
    {
        if (lexem->type == WORD)
        {
            if (!(*cmd)->cmd)
            {
                (*cmd)->cmd = ft_strdup((char *)lexem->value);
                if (!ft_alloc_args(cmd, lexem))
                    return (0);
                if (!ft_fill_args(cmd, &lexem))
                    return (0);
                continue ;
            }
        }
        else if (lexem->type == HEREDOC)
        {
            if (lexem->next && lexem->next->type == WORD)
            {
                (*cmd)->heredoc_end = ft_strdup((char *)lexem->next->value);
                lexem = lexem->next;
            }
        }
        else if (lexem->type == APPEND)
        {
            if (lexem->next && lexem->next->type == WORD)
            {
                (*cmd)->output_file = ft_strdup((char *)lexem->next->value);
                (*cmd)->append = 1;
                lexem = lexem->next;
            }
        }
        else if (lexem->type == IN_REDIRECT)
        {
            if (lexem->next && lexem->next->type == WORD)
            {
                (*cmd)->input_file = ft_strdup((char *)lexem->next->value);
                lexem = lexem->next;
            }
        }
        else if (lexem->type == OUT_REDIRECT)
        {
            if (lexem->next && lexem->next->type == WORD)
            {
                (*cmd)->output_file = ft_strdup((char *)lexem->next->value);
                lexem = lexem->next;
            }
        }
        lexem = lexem->next;
    }
    return (1);
}

int ft_create_command_list(t_cmds **cmds, t_lexems **table)
{
    t_cmds  *new_cmd;
    int     i;

    i = 0;
    while (table[i])
    {
        new_cmd = malloc(sizeof(t_cmds));
        if (!new_cmd)
            return (0);
        ft_init_new(&new_cmd);
        ft_get_new_cmd(&new_cmd, table[i]);
        ft_append_new_command(cmds, new_cmd);
        i++;
    }
    return (1);
}
