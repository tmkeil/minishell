/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envs_and_escapes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:52:26 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/27 02:02:09 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strappend(char **str, char c)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(*str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (0);
	if (*str)
	{
		ft_memcpy(new_str, *str, len);
		free(*str);
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	*str = new_str;
	return (1);
}

int	ft_strappend_str(char **str, char *to_append)
{
	size_t	len;
	size_t	append_len;
	char	*new_str;

	len = ft_strlen(*str);
	append_len = ft_strlen(to_append);
	new_str = malloc(len + append_len + 1);
	if (!new_str)
		return (0);
	if (*str)
	{
		ft_memcpy(new_str, *str, len);
		free(*str);
	}
	if (to_append)
		ft_memcpy(new_str + len, to_append, append_len);
	new_str[len + append_len] = '\0';
	*str = new_str;
	return (1);
}

int	ft_expand_single_quotes(char **expanded, char *current, size_t *i)
{
	while (current[*i])
	{
		ft_strappend(expanded, current[*i]);
		(*i)++;
	}
	return (1);
}

int	ft_expand_escapes(char **expanded, char *current, size_t *i)
{
	(*i)++;
	if (current[*i] == '\\' || current[*i] == '\"'
		|| current[*i] == '\'' || current[*i] == '$')
		return (ft_strappend(expanded, current[(*i)++]));
	else if (current[*i] == '\0')
		return (ft_invalid("\\"), 0);
	else
		return (ft_strappend(expanded, current[(*i)++]));
}

int	ft_expand_environments(char **expanded, char *current, t_envs *envs, size_t *i)
{
	char		*env_start;
	char		*env_end;
	char		*env_var;
	char		*env_value;

	env_start = &current[*i + 1];
	env_end = ft_find_end(env_start);
	if (env_end - env_start > 0)
	{
		env_var = ft_substr(env_start, 0, env_end - env_start);
		env_value = ft_get_env(env_var, envs);
		if (env_value)
			ft_strappend_str(expanded, env_value);
		free(env_var);
		*i += (env_end - env_start) + 1;
		return (1);
	}
	else
		return (ft_strappend(expanded, '$'), (*i)++, 1);
}

int	ft_expander(t_expander *vars, t_types type, size_t *i)
{
    if (type == SINGLE_QUOTE)
        return (ft_expand_single_quotes(vars->expanded, vars->current, i));
    else if (vars->current[*i] && vars->current[*i] == '\\')
        return (ft_expand_escapes(vars->expanded, vars->current, i));
    else if (vars->current[*i] && vars->current[*i] == '$')
        return (ft_expand_environments(vars->expanded, vars->current, vars->envs, i));
    else
        return (ft_strappend(vars->expanded, vars->current[(*i)++]));
}

int ft_expand_token(t_lexems *lex, t_envs *envs)
{
    size_t           i;
    char            *expanded;
    t_expander		vars;

    i = 0;
    expanded = ft_strdup("");
    if (!expanded)
        return (0);
    vars.expanded = &expanded;
    vars.current = (char *)lex->value;
    vars.envs = envs;
    while (vars.current[i])
    {
        if (!ft_expander(&vars, lex->type, &i))
            return (free(expanded), 0);
    }
    free(lex->value);
    lex->value = expanded;
    return (1);
}

int ft_expand_escape_sequences_and_environment_variables(t_lexems **tokens, t_envs *envs)
{
    t_lexems *lex;

    lex = *tokens;
    while (lex)
    {
        if (!ft_expand_token(lex, envs))
            return (0);
        lex = lex->next;
    }
    return (1);
}
