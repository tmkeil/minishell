/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:57:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/22 15:45:20 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_seperator(char **prompt)
{
	if (!*prompt)
		return (1);
	if (ft_is_sep(**prompt))
	{
		while (ft_is_sep(**prompt))
			(*prompt)++;
		return (1);
	}
	return (0);
}

int	ft_handle_identifier(t_lexems **lexems, char **prompt)
{
	char	*ptr;
	char	*sub;
	char	*type;

	if (!*prompt)
		return (1);
	if (ft_is_ident(**prompt))
	{
		type = *prompt;
		ptr = ++(*prompt);
		if (**prompt && **prompt == *type && *prompt - type <= 1)
			ft_append_identifier(lexems, "", *type);
		while (**prompt && **prompt != *type)
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		if (!sub)
			return (0);
		ft_append_identifier(lexems, sub, *type);
		free(sub);
		(*prompt)++;
	}
	return (1);
}

int	ft_handle_operator(t_lexems **lexems, char **prompt)
{
	char	*ptr;
	char	*sub;

	if (!*prompt)
		return (1);
	if (ft_is_op(**prompt))
	{
		ptr = *prompt;
		while (**prompt && ft_is_op(**prompt))
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		if (!sub)
			return (0);
		if (!ft_strnstr(OPERATIONS, sub, ft_strlen(OPERATIONS)))
			return (ft_handle_invalid_operation(sub), free(sub), 0);
		ft_append_operation(lexems, sub);
		free(sub);
	}
	return (1);
}
