/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:04:25 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/12 00:15:20 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_ident(char c)
{
	return (c == '(' || c == ')' || c == '\'' || c == '\"');
}

bool	ft_sep(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_op(char c)
{
	return (ft_strchr(OPERATIONS, c) != NULL);
}

void	ft_putstrn_fd(char *s, int n, int fd)
{
	int	i;

	if (s)
	{
		i = 0;
		while (i < n)
		{
			if (s[i])
				ft_putchar_fd(s[i], fd);
			i++;
		}
	}
}

void	ft_invalid(char *sub, t_minishell **minishell)
{
	int	a;
	int	i;
	int	j;

	a = 0;
	i = 0;
	if (!sub)
		return ;
	while (OPERATIONS[i])
	{
		j = 0;
		while (sub[j] && sub[j] == OPERATIONS[i + j])
			j++;
		a = j * (j > a) + a * (j <= a);
		i++;
	}
	ft_putstr_fd("-bash: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstrn_fd(sub + a, 2, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	(*minishell)->exit_status = 258;
}
