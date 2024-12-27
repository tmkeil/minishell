/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:36:14 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/27 16:58:46 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strappend(char **str, char c)
{
	size_t	len;
	char	*new_str;

	if (!str || !*str)
		return (0);
	len = ft_strlen(*str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (0);
	ft_memcpy(new_str, *str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(*str);
	*str = new_str;
	return (1);
}

int	ft_join(char **str, char *to_join)
{
	char	*tmp;

	if (!str || !to_join)
		return (0);
	tmp = *str;
	*str = ft_strjoin(*str, to_join);
	free(tmp);
	if (!*str)
		return (0);
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
			ft_join(expanded, env_value);
		free(env_var);
		*i += (env_end - env_start) + 1;
		return (1);
	}
	else
		return (ft_strappend(expanded, '$'), (*i)++, 1);
}
