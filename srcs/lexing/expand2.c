/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:36:14 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/30 19:01:48 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_expand_tilde(char **expanded, char *current, size_t *i)
{
	int		c;
	char	*sub;
	char	*user;
	
	c = *i;
	user = getenv("USER");
	if (current[c + 1] == ' ' || current[c + 1] == '/')
		return ((*i)++, ft_join(expanded, getenv("HOME")), 1);
	while (current[c + 1] && ft_isalnum(current[c + 1]))
		c++;
	sub = ft_substr(current, 1, &current[c + 1] - current);
	if (!sub)
		return (0);
	if (!ft_strncmp(sub, user, ft_strlen(sub)))
	{
		ft_join(expanded, getenv("HOME"));
		return ((*i) += (c + 1), free(sub), 1);
	}
	return (ft_strappend(expanded, current[(*i)++]), 1);
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
			ft_join(expanded, env_value);
		free(env_var);
		*i += (env_end - env_start) + 1;
		return (1);
	}
	else
		return (ft_strappend(expanded, '$'), (*i)++, 1);
}
