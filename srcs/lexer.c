/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/16 22:08:47 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test_lexes(t_lexems *lex)
{
	int		i;
	char	*types[] = {[OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
			[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
			[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
			[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
			[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
			[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
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

void	append_lexem(t_lexems **lexems, t_types type, void *value)
{
	t_lexems	*lex;
	t_lexems	*last;

	if (!lexems)
		return ;
	lex = malloc(sizeof(t_lexems));
	if (!lex)
		return ;
	lex->type = type;
	lex->value = ft_strdup((char *)value);
	lex->next = NULL;
	if (!*lexems)
	{
		*lexems = lex;
		return ;
	}
	last = *lexems;
	while (last->next)
		last = last->next;
	last->next = lex;
	return ;
}

void	append_word(t_lexems **lexems, char *sub)
{
	if (ft_isalnum(*sub) || strchr("_/.-$~#+", *sub))
		append_lexem(lexems, WORD, sub);
	else
		append_lexem(lexems, INVALID, sub);
}

void	append_identifier(t_lexems **lexems, char *sub, char type)
{
	if (type == '(')
		append_lexem(lexems, O_BRACKET, sub);
	else if (type == '\'')
		append_lexem(lexems, SINGLE_QUOTE, sub);
	else if (type == '\"')
		append_lexem(lexems, DOUBLE_QUOTE, sub);
	else
		append_lexem(lexems, INVALID, sub);
}

void	append_operation(t_lexems **lexems, char *sub)
{
	if (!strncmp(sub, "&&", 2))
		append_lexem(lexems, AND, "&&");
	else if (!strncmp(sub, "||", 2))
		append_lexem(lexems, OR, "||");
	else if (!strncmp(sub, ">>", 2))
		append_lexem(lexems, APPEND, ">>");
	else if (!strncmp(sub, "<<", 2))
		append_lexem(lexems, HEREDOC, "<<");
	else if (*sub == '|')
		append_lexem(lexems, PIPE, "|");
	else if (*sub == '&')
		append_lexem(lexems, AMPERSAND, "&");
	else if (*sub == '>')
		append_lexem(lexems, OUT_REDIRECT, ">");
	else if (*sub == '<')
		append_lexem(lexems, IN_REDIRECT, "<");
	else
		append_lexem(lexems, INVALID, sub);
}

int	create_lexes(t_lexems **lexems, char *prompt)
{
	char	*ptr;
	char	*sub;

	sub = NULL;
	if (!matches(prompt))
		return (0);
	while (*prompt)
	{
		handle_seperator(&prompt);
		if (!handle_operator(lexems, &prompt))
			return (0);
		if (!handle_identifier(lexems, &prompt))
			return (0);
		ptr = prompt;
		while (*prompt && !is_sep(*prompt) && !is_op(*prompt)
			&& !is_ident(*prompt))
			prompt++;
		if (prompt > ptr)
		{
			sub = ft_substr(ptr, 0, prompt - ptr);
			append_word(lexems, sub);
			free(sub);
		}
	}
	return (ft_test_lexes(*lexems), 1);
}
