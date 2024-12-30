/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2024/12/30 17:29:16 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
// # define OPERATIONS "&& || & | < > << >>"
# define OPERATIONS "| < > << >>"
# define BAD_CD "cd: no such file or directory: "
# define CD_TOO_MANY "-bash: cd: too many arguments"
# define ERR_EXPORT "export: not valid in this context: "
# define BUILTINS "echo cd env exit export pwd unset"

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

typedef struct s_expander
{
	char			*expanded;
	char			*current;
	t_envs			*envs;
}					t_expander;

typedef struct s_lexems
{
	t_types			type;
	void			*value;
	struct s_lexems	*next;
}					t_lexems;

typedef struct s_minishell
{
	int				exit_status;
	char			**envps;
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
void				ft_put_error_str(char *msg, char *value, int *exit_status,
						int *status);
char				*ft_strndup(const char *s, size_t n);
int					ft_update_envps(t_envs *envs, char ***envps);
int					ft_strappend(char **str, char c);
int					ft_join(char **str, char *to_join);
char				**ft_split_once(char *str, char delimiter);

// lexing
int					ft_create_lexes(t_lexems **tokens, char *s, t_envs *envs);
int					ft_op(char c);
bool				ft_sep(char c);
bool				ft_ident(char c);
int					ft_handle_seperator(t_lexems **lexems, char **prompt);
int					ft_handle_operator(t_lexems **lexems, char **prompt);
int					ft_handle_identifier(t_lexems **lexems, char **prompt);
void				ft_append_word(t_lexems **lexems, char *sub);
void				ft_append_operation(t_lexems **lexems, char **sub);
void				ft_append_identifier(t_lexems **lexems, char **sub,
						char type);
void				ft_append_lexem(t_lexems **lexems, t_types type,
						void *value);
void				ft_invalid(char *sub);
// expander
int					ft_expand_escapes_envs(t_lexems **tokens, t_envs *envs);
int					ft_expand_token(t_lexems *lex, t_envs *envs);
int					ft_expander(t_expander *vars, t_types type, size_t *i);
int					ft_expand_single_quotes(char **expanded, char *current,
						size_t *i);
int					ft_expand_escapes(char **expanded, char *current,
						size_t *i);
int					ft_expand_environments(char **expanded, char *current,
						t_envs *envs, size_t *i);

// executing
int					ft_execute_commands(t_minishell **minishell);
char				*ft_getpath(char *cmd, char **envp, bool absolute);
size_t				ft_size(t_lexems *lexes);
void				ft_wait_for_child(t_minishell **minishell, int pid);
char				*ft_find_end(char *ptr);
char				*ft_until_next_env(char *ptr);
char				*ft_get_env(char *env_var, t_envs *envs);
int					ft_print_envs(t_envs *envs);

// cmdarguments
int					ft_handle_lexem(char ***args, t_lexems *lexem, char *cmd);
void				ft_append_args(char **args, char *value);
char				*ft_until_next_env(char *ptr);
char				*ft_find_end(char *ptr);

// builtins
int					ft_changedir(t_minishell **minishell, t_lexems *lexems);
int					ft_export(t_minishell **minishell, t_lexems *lexems, t_envs **envs, char ***envps);
int					ft_unset(t_minishell **minishell, t_lexems *lexems, t_envs **envs, char ***envps);
int					ft_exit(t_minishell **minishell, t_lexems *lexems);
int					ft_pwd(void);
int					ft_env(t_envs *envs);
int					ft_echo(t_lexems *lexem, bool absolute);

// builtin utils
int					ft_set_env(const char *name, const char *value,
						t_envs **envs);

// table
int					ft_create_exec_table(t_minishell **minishell);
void				ft_append_node(t_lexems **table, t_lexems *lex);
size_t				ft_table_size(t_lexems *lexems);

#endif
