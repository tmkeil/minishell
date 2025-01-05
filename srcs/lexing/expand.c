/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:52:26 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/05 12:56:55 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_expander(t_expander *vars, t_types type, size_t *i)
{
	if (type == SINGLE_QUOTE)
		return (ft_expand_single_quotes(&vars->expanded, vars->current, i));
	else if (vars->current[*i] == '\\')
		return (ft_expand_escapes(&vars->expanded, vars->current, i));
	else if (vars->current[*i] == '$')
		return (ft_expand_environments(&vars->expanded, vars->current,
				vars->envs, i));
	else if (vars->current[*i] == '~' && type == WORD)
		return (ft_expand_tilde(&vars->expanded, vars->current, i));
	else
		return (ft_strappend(&vars->expanded, vars->current[(*i)++]));
}

int	ft_expand_token(t_lexems *lex, t_envs *envs)
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
	while (vars.current[i])
	{
		if (!ft_expander(&vars, (lex)->type, &i))
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

int	ft_expand_escapes_envs(t_lexems **tokens, t_envs *envs)
{
	t_lexems	*lex;

	lex = *tokens;
	while (lex)
	{
		if (!ft_expand_token(lex, envs))
			return (0);
		lex = lex->next;
	}
	if (!ft_join_word_neighbours(tokens))
		return (0);
	ft_remove_seperators(tokens);
	return (1);
}
