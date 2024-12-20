/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:04:25 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 20:47:49 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check(char *prompt, char start, char end)
{
	int		count;
	bool	quotes;

	count = 0;
	quotes = true;
	while (*prompt)
	{
		if (start == end && *prompt == start)
			quotes = !quotes;
		else if (*prompt == start)
			count++;
		else if (*prompt == end)
		{
			if (count == 0)
				return (false);
			count--;
		}
		prompt++;
	}
	return (count == 0 && quotes);
}

bool	matches(char *prompt)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (prompt[i])
	{
		if (prompt[i] == '(')
			open++;
		if (prompt[i] == ')' && !open)
		{
			ft_putendl_fd("zsh: parse error near `)'", STDERR_FILENO);
			return (false);
		}
		i++;
	}
	if (!check(prompt, '(', ')'))
		return (false);
	if (!check(prompt, '\'', '\''))
		return (false);
	if (!check(prompt, '\"', '\"'))
		return (false);
	return (true);
}

bool	ft_is_ident(char c)
{
	return (c == '(' || c == ')' || c == '\'' || c == '\"');
}

bool	ft_is_sep(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_is_op(char c)
{
	return (ft_strchr(OPERATIONS, c) != NULL);
}
