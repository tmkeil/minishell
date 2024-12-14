/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/14 18:08:09 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum s_types
{
	OR,
	AND,
	PIPE,
	WORD,
	NUMBER,
	APPEND,
	BRACKET,
	ENV_VAR,
	INVALID,
	REDIRECT,
	SEPARATOR,
	AMPERSAND,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}					t_types;

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>
# include <signal.h>

typedef struct s_lexems
{
	t_types			type;
	void			*value;
	struct s_lexems	*next;
}					t_lexems;

typedef struct s_ast
{
	t_lexems		*lexem;
	struct s_ast	*left;
	struct s_ast	*right;
}				t_ast;

void				display_minishell_intro(void);
int					create_lexes(t_lexems **lexems, char *prompt);

// utils
void				ft_clr(char ***ptr);
void				clr_lexes(t_lexems **lexems);
long				ft_atol(char *s, int *index);

// lexer utils
int					handle_identifier(t_lexems **lexems, char **prompt);
int					handle_seperator(t_lexems **lexems, char **prompt);
char				*create_ident(char c);
bool				is_seperator(char *prompt);
bool				is_ident(char *prompt);
bool				matches(char *prompt);
bool				check(char *prompt, char start, char end);
int					handle_lexem(t_lexems **lexems, char *sub);
int					append_lexem(t_lexems **lexems, t_types type, void *value);

#endif
