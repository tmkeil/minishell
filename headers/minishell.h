/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/16 02:20:56 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define OPERATIONS "&& || & | < > << >>"
typedef enum s_types
{
	OR,
	AND,
	PIPE,
	WORD,//hello, echo, ls, ...
	NUMBER,
	APPEND,//>>
	HEREDOC,//<<
	ENV_VAR,//$
	INVALID,
	LINEFEED,//\n
	O_BRACKET,//(
	C_BRACKET,//)
	AMPERSAND,
	IN_REDIRECT,//<
	OUT_REDIRECT,//>
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

// utils
void				ft_clr(char ***ptr);
void				clr_lexes(t_lexems **lexems);
long				ft_atol(char *s, int *index);

// lexing
int					create_lexes(t_lexems **lexems, char *prompt);
int					is_operator(char c);
bool				is_seperator(char c);
bool				is_identifier(char c);
bool				matches(char *prompt);
bool				check(char *prompt, char start, char end);
int					handle_seperator(char **prompt);
int					handle_operator(t_lexems **lexems, char **prompt);
int					handle_identifier(t_lexems **lexems, char **prompt);
void				append_word(t_lexems **lexems, char *sub);
void				append_operation(t_lexems **lexems, char *sub);
void				append_identifier(t_lexems **lexems, char *sub);
void				append_lexem(t_lexems **lexems, t_types type, void *value);
void				handle_invalid_operation(char *sub);

#endif
