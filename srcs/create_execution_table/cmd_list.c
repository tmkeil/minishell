/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:58:06 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/12 00:51:36 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test_exec_table(t_minishell *minishell)
{
	t_lexems	*current;
	char		*types[] = {[SEPERATOR] = "SEPERATOR", [OR] = "OR",
				[AND] = "AND", [PIPE] = "PIPE",
				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	printf("Testing table:\n");
	for (int i = 0; minishell->table[i]; i++)
	{
		if (!minishell->table[i])
			continue ;
		current = minishell->table[i];
		while (current)
		{
			printf("minishell.table[%i].Type: %s, minishell.table[%i].Value: %s\n", i, types[current->type], i, (char *)current->value ? (char *)current->value : (char *)"(null)");
			current = current->next;
		}
	}
}

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
		return (0);
	}
}

int	ft_append_redirection(t_cmds **cmd, t_lexems **lexem)
{
	if (!(*lexem)->next || (*lexem)->next->type != WORD)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		*lexem = (*lexem)->next;
		return (0);
	}
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
	return (1);
}

int	ft_append_append(t_cmds **cmd, t_lexems **lexem)
{
	if ((*lexem)->next && (*lexem)->next->type == WORD)
	{
		(*cmd)->output_file = ft_strdup((char *)(*lexem)->next->value);
		(*cmd)->append = 1;
		*lexem = (*lexem)->next->next;
	}
	else
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		*lexem = (*lexem)->next;
		return (0);
	}
	return (1);
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

int	ft_get_new_cmd(t_cmds **cmd, t_lexems *lexem)
{
	int	appended;

	while (lexem)
	{
		if (lexem->type == HEREDOC)
		{
			if (!ft_append_heredoc(cmd, &lexem))
				return (0);
			continue ;
		}
		else if (lexem->type == IN_REDIRECT || lexem->type == OUT_REDIRECT)
		{
			if (!ft_append_redirection(cmd, &lexem))
				return (0);
			continue ;
		}
		else if (lexem->type == APPEND)
		{
			if (!ft_append_append(cmd, &lexem))
				return (0);
			continue ;
		}
		else if (lexem->type == WORD)
		{
			appended = ft_append_words(cmd, lexem);
			if (appended == 1)
				continue ;
			else if (appended == 0)
				return (0);
		}
		lexem = lexem->next;
	}
	return (1);
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
		if (!ft_get_new_cmd(&new_cmd, ((*minishell)->table)[i]))
		{
			(*minishell)->exit_status = 258;
			return (ft_validate_commands(*cmds), 0);
		}
		ft_append_new_command(cmds, new_cmd);
		i++;
	}
	return (ft_validate_commands(*cmds));
}
