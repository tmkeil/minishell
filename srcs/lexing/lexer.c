/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 21:03:01 by tkeil            ###   ########.fr       */
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

void	ft_append_lexem(t_lexems **lexems, t_types type, void *value)
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

void	ft_append_word(t_lexems **lexems, char *sub)
{
	if (ft_isalnum(*sub) || strchr("_/.-$~#+", *sub))
		ft_append_lexem(lexems, WORD, sub);
	else
		ft_append_lexem(lexems, INVALID, sub);
}

void	ft_append_identifier(t_lexems **lexems, char *sub, char type)
{
	if (type == '(')
		ft_append_lexem(lexems, O_BRACKET, sub);
	else if (type == '\'')
		ft_append_lexem(lexems, SINGLE_QUOTE, sub);
	else if (type == '\"')
		ft_append_lexem(lexems, DOUBLE_QUOTE, sub);
	else
		ft_append_lexem(lexems, INVALID, sub);
}

void	ft_append_operation(t_lexems **lexems, char *sub)
{
	if (!ft_strncmp(sub, "&&", 2))
		ft_append_lexem(lexems, AND, "&&");
	else if (!ft_strncmp(sub, "||", 2))
		ft_append_lexem(lexems, OR, "||");
	else if (!ft_strncmp(sub, ">>", 2))
		ft_append_lexem(lexems, APPEND, ">>");
	else if (!ft_strncmp(sub, "<<", 2))
		ft_append_lexem(lexems, HEREDOC, "<<");
	else if (*sub == '|')
		ft_append_lexem(lexems, PIPE, "|");
	else if (*sub == '&')
		ft_append_lexem(lexems, AMPERSAND, "&");
	else if (*sub == '>')
		ft_append_lexem(lexems, OUT_REDIRECT, ">");
	else if (*sub == '<')
		ft_append_lexem(lexems, IN_REDIRECT, "<");
	else
		ft_append_lexem(lexems, INVALID, sub);
}

int	ft_create_lexes(t_lexems **tokens, char *prompt)
{
	char	*ptr;
	char	*sub;

	sub = NULL;
	while (*prompt)
	{
		ft_handle_seperator(&prompt);
		if (!ft_handle_operator(tokens, &prompt))
			return (0);
		if (!ft_handle_identifier(tokens, &prompt))
			return (0);
		ptr = prompt;
		while (*prompt && !ft_is_sep(*prompt) && !ft_is_op(*prompt)
			&& !ft_is_ident(*prompt))
			prompt++;
		if (prompt > ptr)
		{
			sub = ft_substr(ptr, 0, prompt - ptr);
			ft_append_word(tokens, sub);
			free(sub);
		}
	}
	return (1);
}
