/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/28 21:33:35 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_append_args(char **args, char *value)
{
	char	*tmp;

	tmp = *args;
	*args = ft_strjoin(*args, value);
	free(tmp);
}

int	ft_handle_lexem(char **args, t_lexems *lexem, char *cmd)
{
	int		i;
	char	**check;

	i = 0;
	check = ft_split(cmd, '/');
	if (!check)
		return (0);
	while (check[i])
		i++;
	if (ft_strncmp("echo", check[i - 1], 5))
		return (ft_echo(lexem), ft_free_ptr(&check), 0);
	while (lexem)
	{
		if (ft_strncmp((char *)lexem->value, " ", 1) != 0)
			ft_append_args(args, (char *)lexem->value);
		lexem = lexem->next;
	}
	return (1);
}
