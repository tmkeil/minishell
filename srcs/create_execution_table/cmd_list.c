/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:58:06 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/06 16:58:46 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    ft_append_heredoc(t_cmds **cmd, t_lexems **lexem)
{
    if ((*lexem)->next && (*lexem)->next->type == WORD)
    {
        (*cmd)->heredoc_end = ft_strdup((char *)(*lexem)->next->value);
        *lexem = (*lexem)->next->next;
		return (1);
    }
	else
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		*lexem = (*lexem)->next;
		return (258);
	}
}

int    ft_append_redir_or_append(t_cmds **cmd, t_lexems **lexem)
{
    if ((*lexem)->type == IN_REDIRECT)
    {
        if ((*lexem)->next && (*lexem)->next->type == WORD)
        {
            (*cmd)->input_file = ft_strdup((char *)(*lexem)->next->value);
            *lexem = (*lexem)->next;
        }
    }
    else if ((*lexem)->type == OUT_REDIRECT)
    {
        if ((*lexem)->next && (*lexem)->next->type == WORD)
        {
            (*cmd)->output_file = ft_strdup((char *)(*lexem)->next->value);
            *lexem = (*lexem)->next;
        }
    }
    else if ((*lexem)->type == APPEND)
    {
        if ((*lexem)->next && (*lexem)->next->type == WORD)
        {
            (*cmd)->output_file = ft_strdup((char *)(*lexem)->next->value);
            (*cmd)->append = 1;
            *lexem = (*lexem)->next;
        }
    }
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

void ft_get_new_cmd(t_cmds **cmd, t_lexems *lexem)
{
    while (lexem)
    {
		// printf("curr lex.value = %s\n", lexem->value);
        if (lexem->type == HEREDOC)
		{
			// printf("here\n");
			ft_append_heredoc(cmd, &lexem);
			continue ;
		}
        else if (lexem->type == IN_REDIRECT || lexem->type == OUT_REDIRECT || lexem->type == APPEND)
		{
			// printf("b\n");
			ft_append_redir_or_append(cmd, &lexem);
			continue ;
		}
        else if (lexem->type == WORD)
        {
			// printf("c\n");
            if (!(*cmd)->cmd)
            {
                (*cmd)->cmd = ft_strdup((char *)lexem->value);
                if (!ft_alloc_args(cmd, lexem))
                    return ;
                if (!ft_fill_args(cmd, &lexem))
                    return ;
                continue ;
            }
        }
        lexem = lexem->next;
    }
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
