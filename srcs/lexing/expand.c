/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:52:26 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 16:36:51 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_test_lexes(t_lexems *lex)
// {
// 	int		i;
// 	char	*types[] = {[SEPERATOR] = "SEPERATOR",
//			[OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
// 			[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
// 			[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
// 			[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
// 			[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
// 			[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
// 			[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
// 			[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

// 	i = 0;
// 	while (lex)
// 	{
// 		printf("lexem[%i].type = %s\n", i, types[lex->type]);
// 		printf("lexem[%i].value = %s\n\n", i, (char *)lex->value);
// 		lex = lex->next;
// 		i++;
// 	}
// }

int	ft_expander(t_minishell **minishell, t_expander *vars,
			size_t *i, t_lexems *lex)
{
	if (vars->type == SINGLE_QUOTE)
		return (ft_expand_single_quotes(&vars, i));
	else if (vars->current[*i] == '\\')
		return (ft_expand_escapes(minishell, &vars, i));
	else if (vars->current[*i] == '$')
		return (ft_expand_environments(&vars, i, lex));
	else if (vars->current[*i] == '~' && vars->type == WORD)
		return (ft_expand_tilde(&vars, i));
	else
		return (ft_strappend(&vars->expanded, vars->current[(*i)++]));
}

int	ft_expand_token(t_minishell **minishell, t_lexems *lex, t_envs *envs)
{
	size_t		i;
	char		*expanded;
	t_expander	vars;

	i = 0;
	expanded = ft_strdup("");
	if (!expanded)
		return (0);
	vars.expanded = expanded;
	vars.current = (char *)(lex)->value;
	vars.envs = envs;
	vars.type = lex->type;
	while (vars.current[i])
	{
		if (!ft_expander(minishell, &vars, &i, lex->next))
			return (free(vars.expanded), 0);
	}
	free(lex->value);
	lex->value = vars.expanded;
	if (lex->type == SINGLE_QUOTE || lex->type == DOUBLE_QUOTE)
		lex->type = WORD;
	return (1);
}

int	ft_join_word_neighbours(t_lexems **tokens)
{
	t_lexems	*lex;
	t_lexems	*to_delete;

	if (!tokens || !*tokens)
		return (0);
	lex = *tokens;
	while (lex && lex->next)
	{
		if (lex->next->type == WORD && lex->type == WORD)
		{
			if (!ft_join_them((char **)&lex->value, (char *)lex->next->value))
				return (0);
			to_delete = lex->next;
			lex->next = to_delete->next;
			lex->type = WORD;
			free(to_delete);
		}
		else
			lex = lex->next;
	}
	return (1);
}

void	ft_remove_seperators(t_lexems **tokens)
{
	t_lexems	*lex;
	t_lexems	*to_delete;

	lex = *tokens;
	while (lex && lex->type == SEPERATOR)
	{
		to_delete = lex;
		lex = lex->next;
		free(to_delete->value);
		free(to_delete);
	}
	*tokens = lex;
	while (lex && lex->next)
	{
		if (lex->next->type == SEPERATOR)
		{
			to_delete = lex->next;
			lex->next = to_delete->next;
			free(to_delete->value);
			free(to_delete);
		}
		else
			lex = lex->next;
	}
}

int	ft_expand_escapes_envs(t_minishell **minishell,
			t_lexems **tokens, t_envs *envs)
{
	t_lexems	*lex;

	lex = *tokens;
	while (lex)
	{
		if (!ft_expand_token(minishell, lex, envs))
			return (0);
		lex = lex->next;
	}
	if (!ft_join_word_neighbours(tokens))
		return (0);
	ft_remove_seperators(tokens);
	ft_remove_invalid_end(tokens);
	return (1);
}
