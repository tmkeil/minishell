/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:13:21 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/14 17:58:26 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ft_atol(char *s, int *index)
{
	int		p;
	long	val;
	bool	valid;

	p = 1;
	val = 0;
	valid = false;
	while (s[*index] && (s[*index] == ' ' || (s[*index] >= 9
				&& s[*index] <= 13)))
		(*index)++;
	if (s[*index] == '-' || s[*index] == '+')
	{
		if (s[*index] == '-')
			p = -1;
		(*index)++;
	}
	while (s[*index] >= '0' && s[*index] <= '9')
	{
		valid = true;
		val = val * 10 + (s[*index] - '0');
		(*index)++;
	}
	if (!valid || !(s[*index] >= 9 && s[*index] <= 13))
		return (LONG_MIN);
	return (val * p);
}

char	*get_command(char *split)
{
	int		i;
	char	*path;
	char	*full;
	char	**env;

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

int check_array_size(char **array)
{
	int i;

	i = 0;
	while(array[i])
	{
		i++;
	}
	return (i);
}