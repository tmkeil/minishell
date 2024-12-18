/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:57:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 15:48:24 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_seperator(char **prompt)
{
	if (!*prompt)
		return (1);
	if (is_sep(**prompt))
	{
		while (is_sep(**prompt))
			(*prompt)++;
		return (1);
	}
	return (0);
}

int	handle_identifier(t_lexems **lexems, char **prompt)
{
	char	*ptr;
	char	*sub;
	char	*type;

	if (!*prompt)
		return (1);
	if (is_ident(**prompt))
	{
		type = *prompt;
		ptr = ++(*prompt);
		if (**prompt && **prompt == *type && *prompt - type <= 1)
			append_identifier(lexems, "", *type);
		while (**prompt && **prompt != *type)
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		append_identifier(lexems, sub, *type);
		free(sub);
		(*prompt)++;
	}
	return (1);
}

int	handle_operator(t_lexems **lexems, char **prompt)
{
	char	*ptr;
	char	*sub;

	if (!*prompt)
		return (1);
	if (is_op(**prompt))
	{
		ptr = *prompt;
		while (**prompt && is_op(**prompt))
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		if (!sub)
			return (0);
		if (!ft_strnstr(OPERATIONS, sub, ft_strlen(OPERATIONS)))
			return (handle_invalid_operation(sub), free(sub), 0);
		append_operation(lexems, sub);
		free(sub);
	}
	return (1);
}

void	ft_putstrn_fd(char *s, int n, int fd)
{
	int	i;

	if (s)
	{
		i = 0;
		while (i < n)
		{
			if (s[i])
				ft_putchar_fd(s[i], fd);
			i++;
		}
	}
}

void	handle_invalid_operation(char *sub)
{
	int	a;
	int	i;
	int	j;

	a = 0;
	i = 0;
	if (!sub)
		return ;
	while (OPERATIONS[i])
	{
		j = 0;
		while (sub[j] && sub[j] == OPERATIONS[i + j])
			j++;
		a = j * (j > a) + a * (j <= a);
		i++;
	}
	ft_putstr_fd("-bash: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstrn_fd(sub + a, 2, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}
