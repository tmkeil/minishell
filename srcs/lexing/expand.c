/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:52:26 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/29 14:14:43 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_expander(t_expander *vars, t_types type, size_t *i)
{
    if (type == SINGLE_QUOTE)
        return (ft_expand_single_quotes(&vars->expanded, vars->current, i));
    else if (vars->current[*i] && vars->current[*i] == '\\')
        return (ft_expand_escapes(&vars->expanded, vars->current, i));
    else if (vars->current[*i] && vars->current[*i] == '$')
        return (ft_expand_environments(&vars->expanded, vars->current, vars->envs, i));
    else
        return (ft_strappend(&vars->expanded, vars->current[(*i)++]));
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
    // printf("vars.expanded: %s\n", vars.expanded);
    // printf("lex->value: %s\n", (char *)lex->value);
    return (1);
}

int ft_expand_escapes_envs(t_lexems **tokens, t_envs *envs)
{
    t_lexems *lex;

    (void)envs;
    lex = *tokens;
    while (lex)
    {
        if (!ft_expand_token(lex, envs))
            return (0);
        lex = lex->next;
    }
    return (1);
}
