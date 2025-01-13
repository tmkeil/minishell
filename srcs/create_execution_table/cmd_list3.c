/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 22:13:39 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 18:23:04 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_validate_commands(t_cmds *cmds)
{
	int	flags;

	if (!cmds)
		return (0);
	while (cmds)
	{
		if (cmds->cmd == NULL && cmds->heredoc)
			return (0);
		if (cmds->input_file)
			if (open(cmds->input_file, O_RDONLY) == -1)
				return (1);
		if (cmds->output_file)
		{
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			if (cmds->append)
				flags = O_WRONLY | O_CREAT | O_APPEND;
			if (open(cmds->output_file, flags, 0644) == -1)
				return (1);
		}
		if (!cmds->cmd && !cmds->output_file
			&& !cmds->input_file && !cmds->heredoc)
			return (0);
		cmds = cmds->next;
	}
	return (1);
}

int	ft_get_new_cmd(t_cmds **cmd, t_lexems *lexem)
{
	int		returned_value;

	while (lexem)
	{
		returned_value = -1;
		if (lexem->type == HEREDOC)
			ft_append_heredoc(cmd, &lexem, &returned_value);
		else if (lexem->type == IN_REDIRECT || lexem->type == OUT_REDIRECT)
			ft_append_redirection(cmd, &lexem, &returned_value);
		else if (lexem->type == APPEND)
			ft_append_append(cmd, &lexem, &returned_value);
		else if (lexem->type == WORD)
			ft_append_words(cmd, lexem, &returned_value);
		if (returned_value == 0)
			return (0);
		else if (returned_value == 1)
			continue ;
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
