/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/13 11:46:40 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_types
{
	NUMBER = 0,
	COMMAND = 1,  // ls
	ARGUMENT = 2, // -l
	REDIRECT = 3, // >, <, >>, <<
	ENV_VAR = 4,  // $
	INVALID = 5,
	OPERATOR = 6, // |, &&, ||, ;
	PATH = 7,     // /bin/ls
	STRING = 8,   // "string" in ""
	SPECIAL = 9,  // &, ;, ', ...
	END_LEXEM = -1
}			e_types;

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_lexems
{
	int		type;
	void	*value;
}			t_lexems;

void		display_minishell_intro(void);
void		create_lexes(char *prompt);

#endif
