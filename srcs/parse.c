/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 22:25:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/13 00:07:46 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long	ft_atol(char *s)
{
	size_t	i;
	int		p;
	long	val;
	bool	valid;

	i = 0;
	p = 1;
	val = 0;
	valid = false;
	while (s[i] && (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13)))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i++] == '-')
			p = -1;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		valid = true;
		val = val * 10 + (s[i++] - '0');
	}
	if (!valid || s[i] != '\0')
		return (LONG_MIN);
	return (val * p);
}

char	*get_command(char *splitted)
{
	int		i;
	char	*path;
	char	*full;
	char	**env;

	path = NULL;
	full = NULL;
	if (access(splitted, X_OK) == 0)
		return (ft_clr(&env), splitted);
	i = 0;
	env = ft_split(getenv("PATH"), ':');
	while (env && env[i])
	{
		path = ft_strjoin(env[i], "/");
		full = ft_strjoin(path, splitted);
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

void	assign(t_lexems **lexems, char *splitted)
{
	long	nbr;

	nbr = ft_atol(splitted);
	if (nbr <= INT_MIN && nbr >= INT_MAX)
	{
		(*lexems)->type = NUMBER;
		(*lexems)->value = (void *)&nbr;
	}
	if (get_command(splitted))
	{
		(*lexems)->type = COMMAND;
		(*lexems)->value = (void *)splitted;
	}
	else
	{
		(*lexems)->type = INVALID;
		(*lexems)->value = NULL;
	}
}

void	create_lexes(char *prompt)
{
	int			i;
	char		**split;
	t_lexems	*lexems;

	split = ft_split(prompt, ' ');
	i = 0;
	while (split[i])
		i++;
	lexems = malloc(sizeof(t_lexems) * (i + 1));
	i = 0;
	while (split[i])
	{
		assign(&lexems[i], split[i]);
		i++;
	}
	lexems[i].type = END_LEXEM;
}
