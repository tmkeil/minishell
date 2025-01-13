/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 22:13:39 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 18:18:08 by tkeil            ###   ########.fr       */
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
		{
			if (open(cmds->input_file, O_RDONLY) == -1)
				return (1);
		}
		if (cmds->output_file)
		{
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			if (cmds->append)
				flags = O_WRONLY | O_CREAT | O_APPEND;
			if (open(cmds->output_file, flags, 0644) == -1)
				return (1);
		}
		if (!cmds->cmd && !cmds->output_file && !cmds->input_file && !cmds->heredoc)
			return (0);
		cmds = cmds->next;
	}
	return (1);
}
