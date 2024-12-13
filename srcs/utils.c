/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:13:21 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/13 19:14:07 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clr_lexes(t_lexems **lexems)
{
	t_lexems	*tmp;

	while (*lexems)
	{
		tmp = (*lexems)->next;
		free((*lexems)->value);
		free(*lexems);
		*lexems = tmp;
	}
	free(*lexems);
	*lexems = NULL;
}

void	ft_clr(char ***ptr)
{
	int	i;

	if (!ptr || !*ptr || !**ptr)
		return ;
	i = 0;
	while ((*ptr)[i])
		free((*ptr)[i++]);
	free(*ptr);
	*ptr = NULL;
}

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
