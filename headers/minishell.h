/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:23:41 by frocha            #+#    #+#             */
/*   Updated: 2025/01/13 18:12:55 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
// # define OPERATIONS "&& || & | < > << >>"
# define OPERATIONS "|<><<>>"
# define BAD_CD "cd: no such file or directory: "
# define CD_TOO_MANY "-bash: cd: too many arguments\n"
# define ERR_EXPORT "bash: export: `"
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
	SEPERATOR,
	O_BRACKET,
	C_BRACKET,
	AMPERSAND,
	IN_REDIRECT,
	OUT_REDIRECT,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}					t_types;

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
	t_types			type;
}					t_expander;

typedef struct s_lexems
{
	t_types			type;
	void			*value;
	struct s_lexems	*next;
}					t_lexems;

typedef struct s_cmds
{
	char			*cmd;
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append;
	char			*heredoc;
	struct s_cmds	*next;
}					t_cmds;

typedef struct s_minishell
{
	int				in_fd;
	int				out_fd;
	int				pipe[2];
	int				exit_status;
	char			**envps;
	t_lexems		*tokens;
	t_lexems		**table;
	t_envs			*envs;
	t_cmds			*cmds;
}					t_minishell;

int					ft_setup_heredoc(t_cmds *cmd);
int					ft_create_command_list(t_minishell **minishell,
						t_cmds **cmds);
void				ft_handle_redirections(t_cmds *cmd, int *in_fd);

// cleaners
void				ft_free_next_iteration(t_minishell **minishell);
void				ft_free_ptr(char ***ptr);
void				ft_free_tokens(t_lexems **lexems);
void				ft_free_table(t_lexems ***table);
void				ft_free_envs(t_envs **head);
void				ft_free_shell(t_minishell **minishell);
void				ft_free_cmds(t_cmds **cmds);

// utils
// minishell utils
int					ft_is_wsl_environment(void);
int					ft_is_interactive(void);
int					ft_set_exit_status(t_minishell *minishell);
char				*ft_get_input(const char *prompt);
char				*ft_get_prompt(void);
// utils
size_t				ft_ptrsize(char **ptr);
void				ft_init_sig(void);
void				ft_set_execution_sig(void);
void				ft_sterr(char *msg1, char *msg2, char *msg3);
// utils2
int					ft_split_env(char *env_var, char **name, char **value);
int					ft_extract_envps(t_envs **envs, char **envp);
int					ft_update_envps(t_envs *envs, char ***envps);
void				ft_print_envs(t_envs *envs);
// utils3
int					ft_join(char **str, char *to_join);
int					ft_strappend(char **str, char c);
char				*ft_get_env(char *env_var, t_envs *envs);

// lexing
int					ft_create_lexes(t_minishell **minishell, t_lexems **tokens,
						char *s, t_envs *envs);
int					ft_op(char c);
bool				ft_sep(char c);
bool				ft_ident(char c);
int					ft_handle_seperator(t_lexems **lexems, char **prompt);
int					ft_handle_operator(t_minishell **minishell,
						t_lexems **lexems,	char **prompt);
int					ft_handle_identifier(t_minishell **minishell,
						t_lexems **lexems, char **prompt);
void				ft_append_word(t_lexems **lexems, char *sub);
void				ft_append_operation(t_lexems **lexems, char **sub);
void				ft_append_identifier(t_lexems **lexems, char **sub,
						char type);
void				ft_append_lexem(t_lexems **lexems, t_types type,
						void *value);
void				ft_invalid(char *sub, t_minishell **minishell);
// expander
int					ft_expand_escapes_envs(t_minishell **minishell,
						t_lexems **tokens, t_envs *envs);
int					ft_expand_token(t_minishell **minishell, t_lexems *lex,
						t_envs *envs);
int					ft_expander(t_minishell **minishell, t_expander *vars,
						size_t *i, t_lexems *lex);
int					ft_expand_single_quotes(t_expander **vars, size_t *i);
int					ft_expand_escapes(t_minishell **minishell,
						t_expander **vars, size_t *i);
int					ft_expand_environments(t_expander **vars, size_t *i,
						t_lexems *lex);
int					ft_expand_tilde(t_expander **vars, size_t *i);
int					ft_join_them(char **value, char *next);
char				*ft_until_next_env(char *ptr);
void				ft_remove_invalid_end(t_lexems **tokens);

// executing
void				ft_close_fd(int *fd);
int					ft_execute_commands(t_minishell **minishell);
char				*ft_getpath(char *cmd, char **envp, char ***args);
char				*ft_find_end(char *ptr);
void				ft_child(t_minishell **minishell, t_cmds *cmd, int *fd_in,
						int *fd_pipe);
int					ft_run_builtin(t_minishell **minishell, t_cmds **cmd,
						int *fd_in, int *fd_pipe);
void				ft_redirect_pipe(int *fd_in, int *fd_pipe, bool is_next);
int					ft_choose_builtin(t_minishell **minishell,
						char *cmd_builtin, char **args);
char				*ft_is_builtin(void *value);
void				ft_execute(t_minishell **minishell, t_cmds *cmd, int *fd_in,
						int *fd_pipe);
void				ft_execute_command_in_pipeline(t_minishell **minishell,
						t_cmds **current, int *fd_in);

// builtins
int					ft_changedir(t_minishell **minishell, char **args);
int					ft_export(t_minishell **minishell, char **args,
						t_envs **envs);
int					ft_unset(t_minishell **minishell, char **args,
						t_envs **envs);
int					ft_exit(t_minishell **minishell, char **args);
int					ft_pwd(void);
int					ft_env(t_envs *envs);
int					ft_echo(char **args);

// builtin utils
int					ft_set_env(const char *name, const char *value,
						t_envs **envs);

// table
int					ft_create_exec_table(t_minishell **minishell);
void				ft_append_node(t_lexems **table, t_lexems *lex);
size_t				ft_table_size(t_lexems *lexems);
int					ft_alloc_args(t_cmds **cmd, t_lexems *lexem);
void				ft_init_new(t_cmds **cmd);
void				ft_append_new_command(t_cmds **cmds, t_cmds *new);
int					ft_validate_commands(t_cmds *cmds);
int					ft_fill_args(t_cmds **cmd, t_lexems **lexem);

void				ft_append_heredoc(t_cmds **cmd, t_lexems **lexem,
						int *returned_value);
void				ft_append_redirection(t_cmds **cmd,
						t_lexems **lexem, int *returned_value);
void				ft_append_append(t_cmds **cmd, t_lexems **lexem,
						int *returned_value);
void				ft_append_words(t_cmds **cmd, t_lexems *lexem,
						int *returned_value);
int					ft_get_new_cmd(t_cmds **cmd, t_lexems *lexem);

#endif
