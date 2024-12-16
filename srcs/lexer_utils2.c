/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:04:25 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/16 02:29:01 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check(char *prompt, char start, char end)
{
	int		count;
	bool 	quotes;

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
	if (!check(prompt, '(', ')'))
		return (false);
	if (!check(prompt, '\'', '\''))
		return (false);
	if (!check(prompt, '\"', '\"'))
		return (false);
	return (true);
}

bool	is_identifier(char c)
{
	return (c == '(' || c == ')' || c == '\'' || c == '\"');
}

bool	is_seperator(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_operator(char c)
{
	return (ft_strchr(OPERATIONS, c) != NULL);
}
