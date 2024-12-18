/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 14:57:49 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 15:09:57 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_find_end(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && (ft_isalnum(ptr[i]) || ptr[i] == '_'))
		i++;
	return (&ptr[i]);
}

char	*ft_until_next_env(char *ptr)
{
	int	i;

	i = 0;
	while (ptr[i] && ptr[i] != '$')
		i++;
	return (&ptr[i]);
}

void	append_to_args(char **args, int i, char *value)
{
	char	*tmp;

	tmp = args[i];
	args[i] = ft_strjoin(args[i], value);
	free(tmp);
}

void	handle_env_var(char **args, int i, char **current)
{
	char	*ptr;
	size_t	len;
	char	*env_var;
	char	*v;

	ptr = *current + 1;
	if (*ptr == '$' || (!ft_isalpha(*ptr) && *ptr != '_'))
	{
		append_to_args(args, i, "$");
		*current += 1 * (*ptr == '$') + (*ptr != '$');
		return ;
	}
	len = ft_find_end(ptr) - ptr;
	env_var = ft_substr(ptr, 0, len);
	v = getenv(env_var);
	if (v)
		append_to_args(args, i, v);
	free(env_var);
	*current += len + 1;
}

void	handle_lexem(char **args, int i, char *current)
{
	size_t	len;
	char	*sub;

	while (*current)
	{
		if (*current != '$')
		{
			len = ft_until_next_env(current) - current;
			sub = ft_substr(current, 0, len);
			append_to_args(args, i, sub);
			free(sub);
			current += len;
		}
		else
			handle_env_var(args, i, &current);
	}
}
