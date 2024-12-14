/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/14 18:04:22 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_lexem(t_lexems **lexems, t_types type, void *value)
{
	t_lexems	*lex;
	t_lexems	*last;

	if (!lexems)
		return (0);
	lex = malloc(sizeof(t_lexems));
	if (!lex)
		return (0);
	lex->type = type;
	if (value)
		lex->value = ft_strdup((char *)value);
	else
		lex->value = NULL;
	lex->next = NULL;
	if (!*lexems)
		return (*lexems = lex, 1);
	last = *lexems;
	while (last->next)
		last = last->next;
	last->next = lex;
	return (1);
}

int	handle_lexem(t_lexems **lexems, char *sub)
{
	if (!ft_strncmp(sub, "||", 2))
		return (append_lexem(lexems, OR, sub));
	else if (!ft_strncmp(sub, "&&", 2))
		return (append_lexem(lexems, AND, sub));
	else if (!ft_strncmp(sub, "|", 1))
		return (append_lexem(lexems, PIPE, sub));
	else if (ft_isdigit(*sub))
		return (append_lexem(lexems, NUMBER, sub));
	else if (ft_isalnum(*sub) || *sub == '_' || *sub == '/')
		return (append_lexem(lexems, WORD, sub));
	else if (!ft_strncmp(sub, ">>", 2) || !ft_strncmp(sub, "<<", 2))
		return (append_lexem(lexems, APPEND, sub));
	else if (!ft_strncmp(sub, "$", 1))
		return (append_lexem(lexems, ENV_VAR, sub));
	else if (!ft_strncmp(sub, ">", 1) || !ft_strncmp(sub, "<", 1))
		return (append_lexem(lexems, REDIRECT, sub));
	else if (!ft_strncmp(sub, "(", 1) || !ft_strncmp(sub, ")", 1))
		return (append_lexem(lexems, BRACKET, sub));
	else if (!ft_strncmp(sub, "&", 1))
		return (append_lexem(lexems, AMPERSAND, sub));
	else if (!ft_strncmp(sub, "\'", 1))
		return (append_lexem(lexems, SINGLE_QUOTE, sub));
	else if (!ft_strncmp(sub, "\"", 1))
		return (append_lexem(lexems, DOUBLE_QUOTE, sub));
	return (append_lexem(lexems, INVALID, sub));
}

void	ft_test_lexes(t_lexems *lex)
{
	int		i;
	char	*types[] = {[OR] = "OR", [AND] = "AND",
			[PIPE] = "PIPE", [WORD] = "WORD", [NUMBER] = "NUMBER",
			[APPEND] = "APPEND", [ENV_VAR] = "ENV_VAR",
			[REDIRECT] = "REDIRECT", [INVALID] = "INVALID",
			[BRACKET] = "BRACKET", [SEPARATOR] = "SEPARATOR",
			[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
			[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	i = 0;
	while (lex)
	{
		printf("lexem[%i].type = %s\n", i, types[lex->type]);
		printf("lexem[%i].value = %s\n\n", i, (char *)lex->value);
		lex = lex->next;
		i++;
	}
}

int	create_lexes(t_lexems **lexems, char *prompt)
{
	char		*ptr;
	char		*sub;

	if (!matches(prompt))
		return (0);
	while (*prompt)
	{
		if (handle_seperator(lexems, &prompt) == 1)
			continue ;
		if (handle_identifier(lexems, &prompt) == 1)
			continue ;
		ptr = prompt;
		while (*prompt && !is_ident(prompt) && !is_seperator(prompt))
			prompt++;
		sub = ft_substr(ptr, 0, prompt - ptr);
		if (!handle_lexem(lexems, sub))
			return (free(sub), 0);
		free(sub);
	}
	ft_test_lexes(*lexems);
	return (1);
}
