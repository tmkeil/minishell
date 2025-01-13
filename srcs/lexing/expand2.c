/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:36:14 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 19:12:19 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_expand_tilde(t_expander **vars, size_t *i)
{
	int		c;
	char	*sub;
	char	*user;


	c = *i;
	user = getenv("USER");
	if ((*vars)->current[c + 1] == ' ' || (*vars)->current[c + 1] == '/')
		return ((*i)++, ft_join(&(*vars)->expanded, getenv("HOME")), 1);
	while ((*vars)->current[c + 1] && ft_isalnum((*vars)->current[c + 1]))
		c++;
	sub = ft_substr((*vars)->current, 1, &(*vars)->current[c + 1]
			- (*vars)->current);
	if (!sub)
		return (0);
	if (!ft_strncmp(sub, user, ft_strlen(sub)))
	{
		ft_join(&(*vars)->expanded, getenv("HOME"));
		return ((*i) += (c + 1), free(sub), 1);
	}
	return (ft_strappend(&(*vars)->expanded, (*vars)->current[(*i)++]),
		free(sub), 1);
}

int	ft_expand_single_quotes(t_expander **vars, size_t *i)
{
	while ((*vars)->current[*i])
	{
		ft_strappend(&(*vars)->expanded, (*vars)->current[*i]);
		(*i)++;
	}
	return (1);
}

int	ft_expand_escapes(t_minishell **minishell, t_expander **vars, size_t *i)
{
	(*i)++;
	if ((*vars)->current[*i] == '\\' || (*vars)->current[*i] == '\"' || (*vars)->current[*i] == '\''
		|| (*vars)->current[*i] == '$')
		return (ft_strappend(&(*vars)->expanded, (*vars)->current[(*i)++]));
	else if ((*vars)->current[*i] == '\0')
		return (ft_invalid("\\", minishell), 0);
	else
		return (ft_strappend(&(*vars)->expanded, (*vars)->current[(*i)++]));
}

int	ft_expand_environments(t_expander **vars, size_t *i, t_lexems *lex)
{
	char	*env_start;
	char	*env_end;
	char	*env_var;
	char	*env_value;

	env_start = &(*vars)->current[*i + 1];
	env_end = ft_find_end(env_start);
	if (!lex && env_end - env_start == 0)
		return (ft_strappend(&(*vars)->expanded, '$'), (*i)++, 1);
	if (env_end - env_start > 0)
	{
		*i += (env_end - env_start) + 1;
		env_var = ft_substr(env_start, 0, env_end - env_start);
		env_value = ft_get_env(env_var, (*vars)->envs);
		if (!env_value)
			return ((*i)++, 1);
		if (!ft_join(&(*vars)->expanded, env_value))
			return (free(env_var), 0);
		free(env_var);
		return (1);
	}
	if ((*vars)->current[*i + 1] == '$' || (*vars)->current[*i + 1] || (!lex && lex->type == WORD))
		return (ft_strappend(&(*vars)->expanded, '$'), (*i)++, 1);
	return ((*i)++, 1);
}
