/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:57:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/14 18:01:14 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_ident(char *prompt)
{
	if (*prompt == '(' || *prompt == ')' || *prompt == '\'' || *prompt == '\"')
		return (true);
	return (false);
}

bool	is_seperator(char *prompt)
{
	if (*prompt == ' ' || (*prompt >= 9 && *prompt <= 13))
		return (true);
	return (false);
}

char	*create_ident(char c)
{
	char	*ident;

	ident = malloc(sizeof(char) * 2);
	ident[0] = c;
	ident[1] = '\0';
	return (ident);
}

int	handle_seperator(t_lexems **lexems, char **prompt)
{
	bool	sep;

	sep = false;
	if (is_seperator(*prompt))
	{
		sep = true;
		append_lexem(lexems, SEPARATOR, (void *)0x0);
		while (is_seperator(*prompt))
			(*prompt)++;
	}
	if (sep)
		return (1);
	return (0);
}

int	handle_identifier(t_lexems **lexems, char **prompt)
{
	bool	ident;
	char	*identifier;

	ident = false;
	identifier = NULL;
	if (is_ident(*prompt))
	{
		ident = true;
		identifier = create_ident(**prompt);
		if (!handle_lexem(lexems, identifier))
			return (free(identifier), 0);
		free(identifier);
		(*prompt)++;
	}
	if (ident)
		return (1);
	return (0);
}
