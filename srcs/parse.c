/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/14 13:47:41 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command(char *split)
{
	int		i;
	char	*path;
	char	*full;
	char	**env;

	path = NULL;
	full = NULL;
	env = NULL;
	if (access(split, X_OK) == 0)
		return (split);
	i = 0;
	env = ft_split(getenv("PATH"), ':');
	while (env && env[i])
	{
		path = ft_strjoin(env[i], "/");
		full = ft_strjoin(path, split);
		free(path);
		if (!full)
			return (ft_clr(&env), NULL);
		if (access(full, X_OK) == 0)
			return (ft_clr(&env), full);
		free(full);
		i++;
	}
	return (NULL);
}

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
		printf("lexem[%i].value = %s\n\n", i, (void *)lex->value);
		lex = lex->next;
		i++;
	}
}

bool	check(char start, char end)
{
	
}

bool	matches(char *prompt)
{
	if (!check('(', ')'))
		return (false);
	if (!check('\'', '\''))
		return (false);
	if (!check('\"', '\"'))
		return (false);
	return (true);
}

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

int	create_lexes(t_lexems **lexems, char *prompt)
{
	char		*ptr;
	char		*sub;

	if (!matches(prompt))
		return (0);
	while (*prompt)
	{
		if (is_seperator(prompt))
		{
			append_lexem(lexems, SEPARATOR, (void *)0x0);
			while (is_seperator(prompt))
				prompt++;
			continue ;
		}
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
