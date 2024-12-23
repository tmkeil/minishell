/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 17:57:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/23 18:03:37 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_seperator(t_lexems **lexems, char **prompt)
{
	if (!*prompt)
		return (1);
	if (ft_sep(**prompt))
	{
		while (ft_sep(**prompt))
			(*prompt)++;
		return (ft_append_word(lexems, " "), 1);
	}
	return (0);
}

int	ft_join(char **sub, char *ptr, char **prompt)
{
	char	*to_join;
	char	*sub2;

	if (!sub || !ptr || !prompt || !*prompt)
		return (0);
	to_join = ft_substr(ptr, 0, *prompt - ptr);
	if (!to_join)
		return (0);
	sub2 = ft_strjoin(*sub, to_join);
	free(*sub);
	free(to_join);
	*sub = NULL;
	if (!sub2)
		return (0);
	*sub = sub2;
	return (1);
}

int	ft_handle_identifier(t_lexems **lexems, char **prompt)
{
	char	*ptr;
	char	*sub;
	char	*type;

	if (!prompt || !*prompt || !lexems)
		return (0);
	while (**prompt == '\'' || **prompt == '"')
	{
		sub = ft_strdup("");
		if (!sub)
			return (0);
		type = *prompt;
		ptr = ++(*prompt);
		while (**prompt && **prompt != *type)
			(*prompt)++;
		if (**prompt != *type)
			return (ft_invalid(type), free(sub), 0);
		if (!ft_join(&sub, ptr, prompt))
			return (free(sub), 0);
		ft_append_identifier(lexems, &sub, *type);
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
	if (ft_op(**prompt))
	{
		ptr = *prompt;
		while (**prompt && ft_op(**prompt))
			(*prompt)++;
		sub = ft_substr(ptr, 0, *prompt - ptr);
		if (!sub)
			return (0);
		if (!ft_strnstr(OPERATIONS, sub, ft_strlen(OPERATIONS)))
			return (ft_invalid(sub), free(sub), 0);
		ft_append_operation(lexems, &sub);
	}
	return (1);
}

// void	handle_escape_sequences(char **prompt, char **output)
// {
// 	if (**prompt == '\\')
// 	{
// 		(*prompt)++;
// 		if (**prompt == '\\' || **prompt == '"' || **prompt == '\''
// 			|| **prompt == '\0')
// 			return (ft_appendchar(output, *(*prompt)++), 1);
// 		else
// 		{
// 			return (ft_invalid("\\"), 0);
// 		}
// 	}
// }
