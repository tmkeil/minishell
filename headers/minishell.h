/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/20 13:24:15 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
// # define OPERATIONS "&& || & | < > << >>"
# define OPERATIONS "| < > << >>"

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

typedef enum s_errors
{
	INVALID_CMD = 127,
}						t_errors;

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
}						t_types;

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

typedef struct s_envs
{
	char				*name;
	char				*value;
	struct s_env_node	*next;
}						t_envs;

typedef struct s_lexems
{
	t_types				type;
	void				*value;
	struct s_lexems		*next;
}						t_lexems;

typedef struct s_minishell
{
	int					exit_status;
	t_lexems			*tokens;
	t_lexems			**table;
	t_envs				*envs;
}						t_minishell;

void					display_minishell_intro(void);

// cleaners
void					ft_clr(char ***ptr);
void					clr_lexes(t_lexems **lexems);
void					clr_exec_table(t_lexems ***table);
void					clean_args(char **args);
void					free_env_list(t_envs *head);

// utils
void					configure_terminal(void);
void					handle_sigquit(int sig);
void					handle_sigint(int sig);
void					split_env_var(const char *env_var, char **name,
							char **value);
t_envs					*extract_envps(t_envs *envs, char **envp);

// lexing
int						create_lexes(t_lexems **lexems, char *prompt);
int						is_op(char c);
bool					is_sep(char c);
bool					is_ident(char c);
bool					matches(char *prompt);
bool					check(char *prompt, char start, char end);
int						handle_seperator(char **prompt);
int						handle_operator(t_lexems **lexems, char **prompt);
int						handle_identifier(t_lexems **lexems, char **prompt);
void					append_word(t_lexems **lexems, char *sub);
void					append_operation(t_lexems **lexems, char *sub);
void					append_identifier(t_lexems **lexems, char *sub,
							char type);
void					append_lexem(t_lexems **lexems, t_types type,
							void *value);
void					handle_invalid_operation(char *sub);

// exe
int						execute_commands(t_lexems **table, char **envp,
							t_envs *envp_list);
char					*ft_getpath(char *cmd, char **envp);

// cmd arguments
void					handle_lexem(char **args, int i, char *current,
							t_types type, t_envs *envp_list);
void					handle_env_var(char **args, int i, char **current,
							t_envs *envp_list);
void					append_to_args(char **args, int i, char *value);
char					*ft_until_next_env(char *ptr);
char					*ft_find_end(char *ptr);

// builtins
int						ft_check_builtin(t_lexems *lexems, char **envp,
							t_envs *envp_list);
int						ft_changedir(t_lexems *lexems);
int						ft_handle_export(t_lexems *args, t_envs *envp_list);
int						ft_unset(t_lexems *lexems, t_envs *envp_list);

// create exe table
int						create_exec_table(t_minishell *minishell);
void					ft_append_node(t_lexems **table, t_lexems *lex);
size_t					ft_table_size(t_lexems *lexems);

#endif
