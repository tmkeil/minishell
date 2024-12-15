/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/15 14:52:12 by tkeil            ###   ########.fr       */
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
	if (ft_isdigit(*sub))
		return (append_lexem(lexems, NUMBER, sub));
	if (ft_isalnum(*sub) || *sub == '_' || *sub == '/')
		return (append_lexem(lexems, WORD, sub));
	if (!ft_strncmp(sub, "$", ft_strlen(sub)))
		return (append_lexem(lexems, ENV_VAR, sub));
	if (!ft_strncmp(sub, "(", ft_strlen(sub)))
		return (append_lexem(lexems, O_BRACKET, sub));
	if (!ft_strncmp(sub, ")", ft_strlen(sub)))
		return (append_lexem(lexems, C_BRACKET, sub));
	if (!ft_strncmp(sub, "\'", ft_strlen(sub)))
		return (append_lexem(lexems, SINGLE_QUOTE, sub));
	if (!ft_strncmp(sub, "\"", ft_strlen(sub)))
		return (append_lexem(lexems, DOUBLE_QUOTE, sub));
	if (!ft_strncmp(sub, "\n", ft_strlen(sub)))
		return (append_lexem(lexems, LINEFEED, sub));
	return (append_lexem(lexems, INVALID, sub));
}

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

int	handle_operator(t_lexems **lexems, char **prompt)
{
	if (!strncmp(*prompt, "&&", 2))
		return (append_lexem(lexems, AND, "&&"), (*prompt += 2), 1);
	if (!strncmp(*prompt, "||", 2))
		return (append_lexem(lexems, OR, "||"), (*prompt += 2), 1);
	if (!strncmp(*prompt, ">>", 2))
		return (append_lexem(lexems, APPEND, ">>"), (*prompt += 2), 1);
	if (!strncmp(*prompt, "<<", 2))
		return (append_lexem(lexems, HEREDOC, "<<"), (*prompt += 2), 1);
	if (**prompt == '|')
		return (append_lexem(lexems, PIPE, "|"), (*prompt)++, 1);
	if (**prompt == '&')
		return (append_lexem(lexems, AMPERSAND, "&"), (*prompt)++, 1);
	if (**prompt == '>')
		return (append_lexem(lexems, OUT_REDIRECT, ">"), (*prompt)++, 1);
	if (**prompt == '<')
		return (append_lexem(lexems, IN_REDIRECT, "<"), (*prompt)++, 1);
	return (0);
}

int	create_lexes(t_lexems **lexems, char *prompt)
{
	char		*ptr;
	char		*sub;

	if (!matches(prompt))
		return (0);
	while (*prompt)
	{
		if (handle_seperator(&prompt) || handle_operator(lexems, &prompt) ||
			handle_identifier(lexems, &prompt))
			continue ;
		ptr = prompt;
		while (*prompt && !is_ident(*prompt) && !is_seperator(*prompt))
			prompt++;
		sub = ft_substr(ptr, 0, prompt - ptr);
		if (!handle_lexem(lexems, sub))
			return (free(sub), 0);
		free(sub);
	}
	ft_test_lexes(*lexems);
	return (1);
}
