/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:58:06 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/11 14:42:44 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_append_heredoc(t_cmds **cmd, t_lexems **lexem)
{
	if ((*lexem)->next && (*lexem)->next->type == WORD)
	{
		(*cmd)->heredoc = ft_strdup((char *)(*lexem)->next->value);
		*lexem = (*lexem)->next->next;
		return (1);
	}
	else
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		*lexem = (*lexem)->next;
		return (258);
	}
}

void	ft_append_redir_or_append(t_cmds **cmd, t_lexems **lexem)
{
	if ((*lexem)->type == IN_REDIRECT)
	{
		if ((*lexem)->next && (*lexem)->next->type == WORD)
		{
			(*cmd)->input_file = ft_strdup((char *)(*lexem)->next->value);
			*lexem = (*lexem)->next->next;
		}
	}
	else if ((*lexem)->type == OUT_REDIRECT)
	{
		if ((*lexem)->next && (*lexem)->next->type == WORD)
		{
			(*cmd)->output_file = ft_strdup((char *)(*lexem)->next->value);
			*lexem = (*lexem)->next->next;
		}
	}
	else if ((*lexem)->type == APPEND)
	{
		if ((*lexem)->next && (*lexem)->next->type == WORD)
		{
			(*cmd)->output_file = ft_strdup((char *)(*lexem)->next->value);
			(*cmd)->append = 1;
			*lexem = (*lexem)->next->next;
		}
	}
}

int	ft_append_words(t_cmds **cmd, t_lexems *lexem)
{
	if (!(*cmd)->cmd)
	{
		(*cmd)->cmd = ft_strdup((char *)lexem->value);
		if (!ft_alloc_args(cmd, lexem))
			return (0);
		if (!ft_fill_args(cmd, &lexem))
			return (0);
		return (1);
	}
	return (2);
}

void	ft_get_new_cmd(t_cmds **cmd, t_lexems *lexem)
{
	int	appended;

	while (lexem)
	{
		if (lexem->type == HEREDOC)
		{
			ft_append_heredoc(cmd, &lexem);
			continue ;
		}
		else if (lexem->type == IN_REDIRECT || lexem->type == OUT_REDIRECT
			|| lexem->type == APPEND)
		{
			ft_append_redir_or_append(cmd, &lexem);
			continue ;
		}
		else if (lexem->type == WORD)
		{
			appended = ft_append_words(cmd, lexem);
			if (appended == 1)
				continue ;
			else if (appended == 0)
				return ;
		}
		lexem = lexem->next;
	}
}

int	ft_create_command_list(t_minishell **minishell, t_cmds **cmds)
{
	t_cmds	*new_cmd;
	int		i;

	i = 0;
	if (!ft_create_exec_table(minishell))
		return (0);
	while (((*minishell)->table)[i])
	{
		new_cmd = malloc(sizeof(t_cmds));
		if (!new_cmd)
			return (0);
		ft_init_new(&new_cmd);
		ft_get_new_cmd(&new_cmd, ((*minishell)->table)[i]);
		ft_append_new_command(cmds, new_cmd);
		i++;
	}
	return (ft_validate_commands(*cmds));
}
