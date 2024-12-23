/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_envs_and_escapes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:52:26 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/23 17:29:55 by tkeil            ###   ########.fr       */
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

int	ft_expand_escape_sequences_and_environment_variables(t_lexems **tokens,
		t_envs *envs)
{
	t_lexems	*lex;
	char		*expanded_value;
	char		*current;
	size_t		i;
	char		*env_start;
	char		*env_end;
	char		*env_var;
	char		*env_value;

	lex = *tokens;
	while (lex)
	{
		expanded_value = ft_strdup("");
		if (!expanded_value)
			return (0);
		i = 0;
		current = (char *)lex->value;
		while (current[i])
		{
			if (current[i] == '\\')
			{
				i++;
				if (current[i] == '\\' || current[i] == '"'
					|| current[i] == '\'' || current[i] == '$')
				{
					ft_strappend(&expanded_value, current[i]);
					i++;
				}
				else if (current[i] == '\0')
					return (ft_invalid("\\"), 0);
				else
					ft_strappend(&expanded_value, current[i++]);
			}
			else if (current[i] == '$')
			{
				env_start = &current[i + 1];
				env_end = ft_find_end(env_start);
				if (env_end - env_start > 0)
				{
					env_var = ft_substr(env_start, 0, env_end - env_start);
					env_value = ft_get_env(env_var, envs);
					if (env_value)
						ft_strappend_str(&expanded_value, env_value);
					free(env_var);
					i += (env_end - env_start) + 1;
				}
				else
				{
					ft_strappend(&expanded_value, '$');
					i++;
				}
			}
			else
				ft_strappend(&expanded_value, current[i++]);
		}
		free(lex->value);
		lex->value = expanded_value;
		lex = lex->next;
	}
	return (1);
}
