/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/16 02:42:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test_lexes(t_lexems *lex)
{
	int		i;
	char	*types[] = {[OR] = "OR", [AND] = "AND",
			[PIPE] = "PIPE", [WORD] = "WORD", [NUMBER] = "NUMBER",
			[APPEND] = "APPEND", [HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
			[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT", [INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
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
	if (value)
		lex->value = ft_strdup((char *)value);
	else
		lex->value = NULL;
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
	if (ft_isalnum(*sub) || *sub == '_' || *sub == '/')
		append_lexem(lexems, WORD, sub);
	else
		append_lexem(lexems, INVALID, sub);
}

void	append_identifier(t_lexems **lexems, char *sub)
{
	if (!ft_strncmp(sub, "(", ft_strlen(sub)))
		append_lexem(lexems, O_BRACKET, sub);
	if (!ft_strncmp(sub, ")", ft_strlen(sub)))
		append_lexem(lexems, C_BRACKET, sub);
	if (!ft_strncmp(sub, "\'", ft_strlen(sub)))
		append_lexem(lexems, SINGLE_QUOTE, sub);
	if (!ft_strncmp(sub, "\"", ft_strlen(sub)))
		append_lexem(lexems, DOUBLE_QUOTE, sub);
	append_lexem(lexems, INVALID, sub);
}

void	append_operation(t_lexems **lexems, char *sub)
{
	if (!strncmp(sub, "&&", 2))
		append_lexem(lexems, AND, "&&");
	if (!strncmp(sub, "||", 2))
		append_lexem(lexems, OR, "||");
	if (!strncmp(sub, ">>", 2))
		append_lexem(lexems, APPEND, ">>");
	if (!strncmp(sub, "<<", 2))
		append_lexem(lexems, HEREDOC, "<<");
	if (*sub == '|')
		append_lexem(lexems, PIPE, "|");
	if (*sub == '&')
		append_lexem(lexems, AMPERSAND, "&");
	if (*sub == '>')
		append_lexem(lexems, OUT_REDIRECT, ">");
	if (*sub == '<')
		append_lexem(lexems, IN_REDIRECT, "<");
}

int	create_lexes(t_lexems **lexems, char *prompt)
{
	char		*ptr;
	char		*sub;

	sub = NULL;
	if (!matches(prompt))
		return (0);
	while (*prompt)
	{
		handle_seperator(&prompt);
		if (!handle_operator(lexems, &prompt) || !handle_identifier(lexems, &prompt))
			return (0);
		ptr = prompt;
		while (*prompt && !is_seperator(*prompt) && !is_operator(*prompt) && !is_identifier(*prompt))
			prompt++;
		if (prompt > ptr)
		{
			sub = ft_substr(ptr, 0, prompt - ptr);
			append_word(lexems, sub);
			free(sub);
		}
	}
	ft_test_lexes(*lexems);
	return (1);
}
