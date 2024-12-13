/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/13 15:44:01 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command(char *split, int *index)
{
	int i;
	char *path;
	char *full;
	char **env;

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
	last = ft_lstlast(*lexems);
	return (last->next = lex, 1);
}

int	handle_lexem(t_lexems *lexems, char *sub)
{
	if (!ft_strncmp(sub, "||", 2))
		return (append_lexem_to_lexems(&lexems, OR, sub));
	else if (!ft_strncmp(sub, "&&", 2))
		return (append_lexem_to_lexems(&lexems, AND, sub));
	else if (!ft_strncmp(sub, "|", 1))
		return (append_lexem_to_lexems(&lexems, PIPE, sub));
	else if (ft_isalnum(*sub) || *sub == '_' || *sub == '/')
		return (append_lexem_to_lexems(&lexems, WORD, sub));
	else if (ft_isdigit(*sub))
		return (append_lexem_to_lexems(&lexems, NUMBER, sub));
	else if (!ft_strncmp(sub, ">>", 2) || !ft_strncmp(sub, "<<", 2))
		return (append_lexem_to_lexems(&lexems, APPEND, sub));
	else if (!ft_strncmp(sub, "$", 1))
		return (append_lexem_to_lexems(&lexems, ENV_VAR, sub));
	else if (!ft_strncmp(sub, ">", 1) || !ft_strncmp(sub, "<", 1))
		return (append_lexem_to_lexems(&lexems, REDIRECT, sub));
	else if (!ft_strncmp(sub, ";", 1))
		return (append_lexem_to_lexems(&lexems, SEMICOLON, sub));
	else if (!ft_strncmp(sub, "&", 1))
		return (append_lexem_to_lexems(&lexems, AMPERSAND, sub));
	else if (!ft_strncmp(sub, "\'", 1))
		return (append_lexem_to_lexems(&lexems, SINGLE_QUOTE, sub));
	else if (!ft_strncmp(sub, "\"", 1))
		return (append_lexem_to_lexems(&lexems, DOUBLE_QUOTE, sub));
	return (append_lexem_to_lexems(&lexems, INVALID, sub));
}

void	ft_test_lexes(t_lexems *lex)
{
	int		i;
	char	*types[] = {[OR] = "OPERATOR", [AND] = "OPERATOR",
			[PIPE] = "OPERATOR", [WORD] = "ARGUMENT", [NUMBER] = "NUMBER",
			[APPEND] = "REDIRECT", [ENV_VAR] = "ENV_VAR",
			[REDIRECT] = "REDIRECT", [INVALID] = "INVALID",
			[SEMICOLON] = "OPERATOR", [SEPARATOR] = "SEPARATOR",
			[AMPERSAND] = "OPERATOR", [SINGLE_QUOTE] = "STRING",
			[DOUBLE_QUOTE] = "STRING"};

	i = 0;
	while (lex->next)
	{
		printf("lexem[%i].type = %s\n", i, types[lex->type]);
		printf("lexem[%i].value = %s\n\n", i, (char *)lex->value);
		lex = lex->next;
		i++;
	}
}

void	create_lexes(char *prompt)
{
	char		*ptr;
	char		*sub;
	t_lexems	*lexems;

	lexems = NULL;
	while (*prompt)
	{
		if (*prompt == ' ' || (*prompt >= 9 && *prompt <= 13))
		{
			append_lexem_to_lexems(&lexems, SEPARATOR, (void *)SEPARATOR);
			while (*prompt == ' ' || (*prompt >= 9 && *prompt <= 13))
				prompt++;
		}
		ptr = prompt;
		while (*prompt && !(*prompt >= 9 && *prompt <= 13))
			prompt++;
		sub = ft_substr(ptr, 0, prompt - ptr);
		if (!handle_lexem(lexems, sub))
			return (free(sub), 0);
		free(sub);
	}
	ft_test_lexes(lexems);
	return (1);
}
