/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 15:46:35 by frocha            #+#    #+#             */
/*   Updated: 2024/12/19 14:52:59 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clr_ast(t_ast **ast)
{
	if (!ast || !(*ast))
		return ;
	if ((*ast)->left)
		clr_ast(&(*ast)->left);
	if ((*ast)->right)
		clr_ast(&(*ast)->right);
	if ((*ast)->lexem)
		clr_lexes(&(*ast)->lexem);
	free(*ast);
	*ast = NULL;
}

void	clr_lexes(t_lexems **lexems)
{
	t_lexems	*tmp;

	if (!lexems || !(*lexems))
		return ;
	while (*lexems)
	{
		tmp = (*lexems)->next;
		free((*lexems)->value);
		free(*lexems);
		*lexems = tmp;
	}
	*lexems = NULL;
}

void	ft_clr(char ***ptr)
{
	int	i;

	if (!ptr || !*ptr || !**ptr)
		return ;
	i = 0;
	while ((*ptr)[i])
	{
		free((*ptr)[i]);
		i++;
	}
	free(*ptr);
	*ptr = NULL;
}

void	clr_exec_table(t_lexems ***table)
{
	int			i;
	t_lexems	*current;
	t_lexems	*next;

	if (!table || !*table || !**table)
		return ;
	i = 0;
	while ((*table)[i])
	{
		current = (*table)[i++];
		while (current)
		{
			next = current->next;
			free(current->value);
			free(current);
			current = next;
		}
	}
}

void	clean_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
	args = NULL;
}

void free_env_list(t_env_node *head)
{
    t_env_node *current;

    while (head) {
        current = head;
        head = head->next;

        free(current->name);
        free(current->value);
        free(current);
    }
}