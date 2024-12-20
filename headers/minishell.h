/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/21 00:44:01 by tkeil            ###   ########.fr       */
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
	INVALID_CD = 1,
}					t_errors;

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

typedef struct s_envs
{
	char			*name;
	char			*value;
	struct s_envs	*next;
}					t_envs;

typedef struct s_lexems
{
	t_types			type;
	void			*value;
	struct s_lexems	*next;
}					t_lexems;

typedef struct s_minishell
{
	int				exit_status;
	t_lexems		*tokens;
	t_lexems		**table;
	t_envs			*envs;
}					t_minishell;

void				ft_display_intro(void);

// cleaners
void				ft_free_ptr(char ***ptr);
void				ft_free_tokens(t_lexems **lexems);
void				ft_free_table(t_lexems ***table);
void				ft_free_envs(t_envs **head);
void				ft_free_shell(t_minishell **minishell);

// utils
void				ft_configure_terminal(void);
void				ft_handle_sigquit(int sig);
void				ft_handle_sigint(int sig);
int					ft_split_env(const char *env_var, char **name,
						char **value);
int					ft_extract_envps(t_envs **envs, char **envp);

// lexing
int					ft_create_lexes(t_lexems **lexems, char *prompt);
int					ft_is_op(char c);
bool				ft_is_sep(char c);
bool				ft_is_ident(char c);
int					ft_handle_seperator(char **prompt);
int					ft_handle_operator(t_lexems **lexems, char **prompt);
int					ft_handle_identifier(t_lexems **lexems, char **prompt);
void				ft_append_word(t_lexems **lexems, char *sub);
void				ft_append_operation(t_lexems **lexems, char *sub);
void				ft_append_identifier(t_lexems **lexems, char *sub,
						char type);
void				ft_append_lexem(t_lexems **lexems, t_types type,
						void *value);
void				ft_handle_invalid_operation(char *sub);

// executing
int					ft_execute_commands(t_minishell **minishell, char **envp);
char				*ft_getpath(char *cmd, char **envp);
size_t				ft_size(t_lexems *lexes);
void				ft_handle_invalid_command(t_minishell **minishell,
						t_lexems *cmd);
void				ft_wait_for_child(t_minishell **minishell, int pid,
						char *cmd);
char				*ft_find_end(char *ptr);
char				*ft_until_next_env(char *ptr);

// cmdarguments
void				ft_handle_lexem(char **args, int i, char *current,
						t_types type, t_envs *envs);
void				ft_handle_env(char **args, int i, char **current,
						t_envs *envs);
void				ft_append_args(char **args, int i, char *value);
char				*ft_until_next_env(char *ptr);
char				*ft_find_end(char *ptr);

// builtins
int					ft_check_builtin(int index_lexem, char **envp,
						t_minishell **minishell);
int					ft_changedir(t_minishell **minishell, t_lexems *lexems);
int					ft_export(t_lexems *lexems, t_envs **envs);
int					ft_unset(t_lexems *lexems, t_envs **envs);
int					ft_set_env(const char *name, const char *value,
						t_envs **envs);
void				ft_exit(t_lexems *lexems, int index_lexem,
						t_minishell **minishell);

// table
int					ft_create_exec_table(t_minishell **minishell);
void				ft_append_node(t_lexems **table, t_lexems *lex);
size_t				ft_table_size(t_lexems *lexems);

#endif
