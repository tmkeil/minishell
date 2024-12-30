/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/30 16:00:03 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_append_args(char **args, char *value)
{
	char	*tmp;

	tmp = *args;
	*args = ft_strjoin(*args, value);
	if (tmp)
		free(tmp);
}

int	ft_handle_lexem(char ***args, t_lexems *lexem, char *cmd)
{
	int		i;
	char	**ptr;

	i = 0;
	ptr = ft_split(cmd, '/');
	if (!ptr)
		return (0);
	while (ptr[i])
		i++;
	if (!ft_strncmp("echo", ptr[i - 1], 5))
		return (ft_echo(lexem, true), ft_free_ptr(&ptr), 2);
	i = 0;
	while (lexem)
	{
		if (ft_strncmp((char *)lexem->value, " ", 1) != 0)
			ft_append_args(&(*args)[i++], (char *)lexem->value);
		lexem = lexem->next;
	}
	return (1);
}
