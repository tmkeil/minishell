/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:57:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/16 02:42:48 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_seperator(char **prompt)
{
	if (!*prompt)
		return (1);
	if (is_seperator(**prompt))
	{
		while (is_seperator(**prompt))
			(*prompt)++;
		return (1);
	}
	return (0);
}

int	handle_identifier(t_lexems **lexems, char **prompt)
{
	char	*ptr;
	char	*sub;

	ptr = NULL;
	sub = NULL;
	if (!*prompt)
		return (1);
	if (is_identifier(**prompt))
	{
		(*prompt)++;
		ptr = *prompt;
		while (!is_identifier(**prompt))
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		append_identifier(lexems, sub);
		free(sub);
	}
	return (1);
}

int	handle_operator(t_lexems **lexems, char **prompt)
{
	char 	*ptr;
	char	*sub;

	ptr = NULL;
	sub = NULL;
	if (!*prompt)
		return (1);
	if (is_operator(**prompt))
	{
		ptr = *prompt;
		while (is_operator(**prompt))
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		if (!ft_strnstr(OPERATIONS, sub, ft_strlen(OPERATIONS)))
			return (handle_invalid_operation(sub), free(sub), 0);
		append_operation(lexems, sub);
		free(sub);
	}
	return (1);
}

void	handle_invalid_operation(char *sub)
{
	int		a;
	int		i;
	int		j;
	char	*ptr;

	a = 0;
	i = 0;
	ptr = NULL;
	if (!sub)
		return ;
	while (OPERATIONS[i])
	{
		j = 0;
		while (sub[j] && sub[j] == OPERATIONS[i + j])
			j++;
		if (j > a)
			a = j;
		i++;
	}
	if (!ft_strlcpy(ptr, &sub[a], 2))
		return ;
	ft_printf("-bash: syntax error near unexpected token `%s'\n", ptr);
	free(ptr);
}
