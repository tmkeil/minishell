/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:57:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/15 14:50:53 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_ident(char c)
{
	return (c == '(' || c == ')' || c == '\'' || c == '\"' ||
			c == '|' || c == '&' || c == '<' || c == '>');
}

bool	is_seperator(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

char	*create_ident(char c)
{
	char	*ident;

	ident = malloc(sizeof(char) * 2);
	ident[0] = c;
	ident[1] = '\0';
	return (ident);
}

int	handle_seperator(char **prompt)
{
	if (is_seperator(**prompt))
	{
		while (is_seperator(**prompt))
			(*prompt)++;
		return (1);
	}
	return (0);
}

int	handle_identifier(t_lexems **lexems, char **prompt)
{
	char	*identifier;

	identifier = NULL;
	if (is_ident(**prompt))
	{
		identifier = create_ident(**prompt);
		if (!handle_lexem(lexems, identifier))
			return (free(identifier), 0);
		free(identifier);
		(*prompt)++;
		return (1);
	}
	return (0);
}
