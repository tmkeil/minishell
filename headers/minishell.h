/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/13 15:14:03 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_types
{
	OR = 0,
	AND = 1,
	PIPE = 2,
	WORD = 3,
	NUMBER = 4,
	APPEND = 5,
	ENV_VAR = 6,
	REDIRECT = 7,
	INVALID = 8,
	SEMICOLON = 9,
	SEPARATOR = 10,
	AMPERSAND = 11,
	SINGLE_QUOTE = 12,
	DOUBLE_QUOTE = 13
}					e_types;

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_lexems
{
	int				type;
	void			*value;
	struct s_lexems	*next;
}					t_lexems;

void				display_minishell_intro(void);
void				create_lexes(char *prompt);

// utils
void				ft_clr(char ***ptr);
long				ft_atol(char *s, int *index);
#endif
