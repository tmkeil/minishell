/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/01 15:09:27 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_append_args(char **args, char *value)
{
	char	*tmp;

	tmp = *args;
	*args = ft_strjoin(*args, value);
	if (!args)
		return (0);
	if (tmp)
		free(tmp);
	return (1);
}

int	ft_get_args(char ***args, t_lexems *lexem)
{
	int	i;

	i = 0;
	while (lexem)
	{
		if (ft_strnstr(OPERATIONS, (char *)lexem->value, ft_strlen(OPERATIONS)))
			break ;
		if (lexem->type != SEPERATOR)
		{
			if (!ft_append_args(&(*args)[i++], (char *)lexem->value))
				return (0);
		}
		lexem = lexem->next;
	}
	return (1);
}

int	ft_handle_lexem(char ***args, t_lexems *lexem, char *cmd)
{
	int		i;
	char	**ptr;

	i = 0;
	if (!ft_get_args(args, lexem))
		return (ft_free_ptr(args), ft_free_ptr(&ptr), 0);
	ptr = ft_split(cmd, '/');
	if (!ptr)
		return (ft_free_ptr(args), 0);
	while (ptr[i])
		i++;
	if (!ft_strncmp("echo", ptr[i - 1], 5))
	{
		ft_free_ptr(&ptr);
		ft_echo(lexem, args, true);		
	}
	return (ft_free_ptr(&ptr), 1);
}
