/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 19:02:57 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 16:35:08 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_join_them(char **value, char *next)
{
	if (!value || !next)
		return (0);
	if (!ft_join(value, next))
		return (0);
	return (free(next), 1);
}

char	*ft_find_end(char *ptr)
{
	int	i;

	i = 0;
	if (ptr[i] == '?')
		return (&ptr[i + 1]);
	while (ptr[i] && (ft_isalnum(ptr[i]) || ft_strchr("_", ptr[i])))
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

void	ft_remove_invalid_end(t_lexems **tokens)
{
	t_lexems	*lex;
	t_lexems	*to_delete;

	lex = *tokens;
	while (lex && lex->next)
	{
		if (lex->next->type == INVALID)
		{
			to_delete = lex->next;
			lex->next = to_delete->next;
			free(to_delete->value);
			free(to_delete);
		}
		else
			lex = lex->next;
	}
}
