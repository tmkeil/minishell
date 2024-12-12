/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/13 00:26:05 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define NUMBER 0
# define COMMAND 1 // ls
# define ARGUMENT 2 // ls -l
# define REDIRECT 3 // >, <, >>, <<
# define ENV_VAR 4 // $
# define INVALID 5
# define OPERATOR 6 // |
# define PATH 7 // /bin/ls
# define STRING 8 // "string" in ""
# define SPECIAL 9 // &, ;, ', ...
# define END_LEXEM -1

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
