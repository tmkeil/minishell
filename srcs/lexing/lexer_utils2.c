/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:04:25 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 22:09:19 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_is_ident(char c)
{
	return (c == '(' || c == ')' || c == '\'' || c == '\"');
}

bool	ft_is_sep(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_is_op(char c)
{
	return (ft_strchr(OPERATIONS, c) != NULL);
}
