/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_exec_table.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:17:57 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 14:36:01 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_table_size(t_lexems *lexems)
{
	size_t	size;

	size = 0;
	if (!lexems)
		return (0);
	while (lexems)
	{
		if (lexems->type == PIPE)
			size++;
		lexems = lexems->next;
	}
	return (size);
}

void	ft_append_node(t_lexems **table, t_lexems *lex)
{
	t_lexems	*last;
	t_lexems	*new_node;

	new_node = malloc(sizeof(t_lexems));
	if (!new_node)
		return ;
	new_node->type = lex->type;
	new_node->value = ft_strdup((char *)lex->value);
	new_node->next = NULL;
	if (!*table)
	{
		*table = new_node;
		return ;
	}
	last = *table;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return ;
}

// void ft_debug_exec_table(t_minishell *minishell)
// {
// 	char		*types[] = {[SEPERATOR] = "SEP", [OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
// 				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
// 				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
// 				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
// 				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
// 				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
// 				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
// 				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};
// 	int i = 0;
//     for (i = 0; minishell->table[i]; i++)
//     {
//         t_lexems *current = minishell->table[i];
//         while (current)
//         {
//             printf("table[%i].Type: %s, table[%i].Value: %s\n", i, types[current->type], i, (char *)current->value ? (char *)current->value : (char *)"(null)");
//             current = current->next;
//         }
//     }
// 	printf("table[%i] = %p\n", i, minishell->table[i]);
// }

int	ft_skip_prefix_spaces(t_lexems **lexes, bool *first)
{
	if (*first && *lexes)
	{
		*first = false;
		if ((*lexes)->type == SEPERATOR)
			*lexes = (*lexes)->next;
	}
	return (*lexes != NULL);
}

int ft_init_table(t_minishell **minishell, size_t size)
{
    size_t i;

    i = 0;
    (*minishell)->table = malloc(sizeof(t_lexems *) * size);
    if (!(*minishell)->table)
        return (0);
    while (i < size)
        (*minishell)->table[i++] = NULL;
    return (1);
}

int	ft_create_exec_table(t_minishell **minishell)
{
	bool		first;
	size_t		i;
	size_t		size;
	t_lexems	*lex;

	i = 0;
	first = true;
	lex = (*minishell)->tokens;
	size = ft_table_size(lex) + 2;
	if (!ft_init_table(minishell, size))
		return (0);
	while (lex)
	{
		if (!ft_skip_prefix_spaces(&lex, &first))
			return (0);
		if (lex->type != PIPE)
			ft_append_node(&(*minishell)->table[i], lex);
		else
		{
			first = true;
			i++;
		}
		lex = lex->next;
	}
	return (1);
}
