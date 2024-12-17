/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/17 15:07:45 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
// # define OPERATIONS "&& || & | < > << >>"
# define OPERATIONS "| < > << >>"

typedef enum s_types
{
	OR,
	AND,
	PIPE,
	WORD,
	NUMBER,
	APPEND,
	HEREDOC,
	ENV_VAR,
	INVALID,
	LINEFEED,
	O_BRACKET,
	C_BRACKET,
	AMPERSAND,
	IN_REDIRECT,
	OUT_REDIRECT,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}					t_types;

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

typedef struct s_lexems
{
	t_types			type;
	void			*value;
	struct s_lexems	*next;
}					t_lexems;

typedef struct s_exec_table
{
	t_lexems	**lexems;
}				t_exec_table;

typedef struct s_ast
{
	t_lexems		*lexem;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*arguments;
}					t_ast;

void				display_minishell_intro(void);

// cleaners
void				ft_clr(char ***ptr);
void				clr_lexes(t_lexems **lexems);
void				clr_ast(t_ast **ast);
void				clr_exec_table(t_exec_table *exec_table);
void				clean_args(char **args);

// utils
long				ft_atol(char *s, int *index);

// lexing
int					create_lexes(t_lexems **lexems, char *prompt);
int					is_op(char c);
bool				is_sep(char c);
bool				is_ident(char c);
bool				matches(char *prompt);
bool				check(char *prompt, char start, char end);
int					handle_seperator(char **prompt);
int					handle_operator(t_lexems **lexems, char **prompt);
int					handle_identifier(t_lexems **lexems, char **prompt);
void				append_word(t_lexems **lexems, char *sub);
void				append_operation(t_lexems **lexems, char *sub);
void				append_identifier(t_lexems **lexems, char *sub, char type);
void				append_lexem(t_lexems **lexems, t_types type, void *value);
void				handle_invalid_operation(char *sub);

// parsing
void				parse_lexes(t_lexems **lexems);

// exe
int	execute_commands(t_exec_table *exec_table, char **envp);

#endif
