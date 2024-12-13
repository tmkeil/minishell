/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/13 18:31:45 by tkeil            ###   ########.fr       */
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

int	append_lexem_to_lexems(t_lexems **lexems, e_types type, void *value)
{
	t_lexems	*lex;
	t_lexems	*last;

	// printf("value = %s\n", (char *)value);
	if (!lexems)
		return (0);
	lex = malloc(sizeof(t_lexems));
	if (!lex)
		return (0);
	lex->type = type;
	lex->value = value;
	lex->next = NULL;
	if (!*lexems)
		return (*lexems = lex, 1);
	last = *lexems;
	while (last->next)
		last = last->next;
	return (last->next = lex, 1);
}

int	handle_lexem(t_lexems **lexems, char *sub)
{
	if (!ft_strncmp(sub, "||", 2))
		return (printf("OR sub = %s\n", sub), append_lexem_to_lexems(lexems, OR, sub));
	else if (!ft_strncmp(sub, "&&", 2))
		return (printf("AND sub = %s\n", sub), append_lexem_to_lexems(lexems, AND, sub));
	else if (!ft_strncmp(sub, "|", 1))
		return (printf("PIPE\n"), append_lexem_to_lexems(lexems, PIPE, sub));
	else if (ft_isdigit(*sub))
		return (printf("NUM\n"), append_lexem_to_lexems(lexems, NUMBER, sub));
	else if (ft_isalnum(*sub) || *sub == '_' || *sub == '/')
		return (printf("WORD\n"), append_lexem_to_lexems(lexems, WORD, sub));
	else if (!ft_strncmp(sub, ">>", 2) || !ft_strncmp(sub, "<<", 2))
		return (printf("APPEND\n"), append_lexem_to_lexems(lexems, APPEND, sub));
	else if (!ft_strncmp(sub, "$", 1))
		return (printf("ENV\n"), append_lexem_to_lexems(lexems, ENV_VAR, sub));
	else if (!ft_strncmp(sub, ">", 1) || !ft_strncmp(sub, "<", 1))
		return (printf("REDI\n"), append_lexem_to_lexems(lexems, REDIRECT, sub));
	else if (!ft_strncmp(sub, ";", 1))
		return (printf("SEM\n"), append_lexem_to_lexems(lexems, SEMICOLON, sub));
	else if (!ft_strncmp(sub, "&", 1))
		return (printf("AMPER\n"), append_lexem_to_lexems(lexems, AMPERSAND, sub));
	else if (!ft_strncmp(sub, "\'", 1))
		return (printf("SIN\n"), append_lexem_to_lexems(lexems, SINGLE_QUOTE, sub));
	else if (!ft_strncmp(sub, "\"", 1))
		return (printf("SOB\n"), append_lexem_to_lexems(lexems, DOUBLE_QUOTE, sub));
	return (printf("INVALID\n"), append_lexem_to_lexems(lexems, INVALID, sub));
}

void	ft_test_lexes(t_lexems *lex)
{
	int		i;
	char	*types[] = {[OR] = "OR", [AND] = "AND",
			[PIPE] = "PIPE", [WORD] = "WORD", [NUMBER] = "NUMBER",
			[APPEND] = "APPEND", [ENV_VAR] = "ENV_VAR",
			[REDIRECT] = "REDIRECT", [INVALID] = "INVALID",
			[SEMICOLON] = "SEMICOLON", [SEPARATOR] = "SEPARATOR",
			[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
			[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	i = 0;
	// printf("lex calue = %s\n", lex->value);
	// printf("lex type = %d\n", lex->type);
	while (lex)
	{
		printf("lexem[%i].type = %s\n", i, types[lex->type]);
		printf("lexem[%i].value = %s\n\n", i, (void *)lex->value);
		lex = lex->next;
		i++;
	}
}

int	create_lexes(char *prompt)
{
	char		*ptr;
	char		*sub;
	t_lexems	*lexems;

	lexems = NULL;
	while (*prompt)
	{
		// printf("pos at: %s\n", prompt);
		if (*prompt == ' ' || (*prompt >= 9 && *prompt <= 13))
		{
			append_lexem_to_lexems(&lexems, SEPARATOR, (void *)0x0);
			while (*prompt == ' ' || (*prompt >= 9 && *prompt <= 13))
				prompt++;
			// continue ;
		}
		ptr = prompt;
		while (*prompt && *prompt != ' ' && !(*prompt >= 9 && *prompt <= 13))
			prompt++;
		sub = ft_substr(ptr, 0, prompt - ptr);
		printf("sub = %s\n", sub);
		if (!handle_lexem(&lexems, sub))
			return (free(sub), 0);
		free(sub);
	}
	ft_test_lexes(lexems);
	return (1);
}
