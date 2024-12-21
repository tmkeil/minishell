/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 20:29:09 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/21 01:40:18 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_size(t_lexems *lexes)
{
	size_t	i;

	i = 0;
	while (lexes)
	{
		if (lexes->type != PIPE)
			i++;
		lexes = lexes->next;
	}
	return (i);
}

void	ft_handle_invalid_command(t_minishell **minishell, t_lexems *cmd)
{
	(*minishell)->exit_status = INVALID_CMD;
	printf("zsh: command not found: %s\n", (char *)cmd->value);
}

void	ft_wait_for_child(t_minishell **minishell, int pid, char *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*minishell)->exit_status = WEXITSTATUS(status);
	else
	{
		(*minishell)->exit_status = INVALID_CMD;
		ft_printf("zsh: command failed: %s\n", cmd);
	}
}

char	*ft_find_end(char *ptr)
{
	int	i;

	i = 0;
	if (ptr[i] == '?')
		return (&ptr[i + 1]);
	while (ptr[i] && (ft_isalnum(ptr[i]) || ft_strchr("_", ptr[i])))
		i++;
	return (&ptr[i]);
}

char	*ft_until_next_env(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && ptr[i] != '$')
		i++;
	return (&ptr[i]);
}
