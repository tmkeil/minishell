/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 19:31:23 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_table_size(t_lexems *lexems)
{
	size_t	size;

	size = 0;
	if (!lexems)
		return (0);
	while (lexems)
	{
		if (lexems->type == PIPE)
			size++;
		lexems = lexems->next;
	}
	return (size);
}

void	ft_append_node(t_lexems **table, t_lexems *lex)
{
	t_lexems	*last;
	t_lexems	*new_node;

	new_node = malloc(sizeof(t_lexems));
	if (!new_node)
		return ;
	new_node->type = lex->type;
	new_node->value = ft_strdup((char *)lex->value);
	new_node->next = NULL;
	if (!*table)
	{
		*table = new_node;
		return ;
	}
	last = *table;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return ;
}

int	create_exec_table(t_lexems **lexems, t_exec_table *exec_table)
{
	size_t		i;
	size_t		size;
	t_lexems	*lex;

	i = 0;
	lex = *lexems;
	size = ft_table_size(lex) + 2;
	exec_table->lexems = malloc(sizeof(t_lexems *) * size);
	if (!exec_table->lexems)
		return (0);
	while (i < size)
		exec_table->lexems[i++] = NULL;
	i = 0;
	while (lex)
	{
		if (lex->type != PIPE)
			ft_append_node(&exec_table->lexems[i], lex);
		else
			i++;
		lex = lex->next;
	}
	exec_table->lexems[i + 1] = NULL;
	return (1);
}

void	ft_test_exec_table(t_exec_table table)
{
	int			i;
	t_lexems	*current;
	char		*types[] = {[OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	i = 0;
	while (table.lexems[i])
	{
		current = table.lexems[i];
		while (current)
		{
			printf("table.lexems[%i]->type = %s\n", i, types[current->type]);
			printf("table.lexems[%i]->value = %s\n", i, (char *)current->value);
			current = current->next;
		}
		i++;
	}
}

void	get_user_input(char **envp, t_env_node *envp_list)
{
	t_lexems		*lexems;
	t_exec_table	exec_table;
	char			*prompt;
	char			*text_show;

	lexems = NULL;
	text_show = ft_strjoin(getenv("USER"), "@minishell $ ");
	prompt = readline(text_show);
	if (!prompt)
	{
		exit(0);
	}
	add_history(prompt);
	create_lexes(&lexems, prompt);
	create_exec_table(&lexems, &exec_table);
	ft_test_exec_table(exec_table);
	// parse, execute are not there yet
	// parse_lexes(&lexems);
	execute_commands(&exec_table, envp, envp_list);
	clr_exec_table(&exec_table);
	clr_lexes(&lexems);
	free(text_show);
	free(prompt);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_redisplay();
}

void	configure_terminal(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void	start_bash(char **envp, t_env_node *envp_list)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	configure_terminal();
	display_minishell_intro();
	while(1)
	{
		get_user_input(envp, envp_list);
	}
}

void	finish_bash(void)
{
	rl_clear_history();
}

void split_env_var(const char *env_var, char **name, char **value) {
    char *equal_sign;
    size_t name_len;

    equal_sign = ft_strchr(env_var, '=');
    if (equal_sign) {
        name_len = equal_sign - env_var;
        *name = malloc(name_len + 1);
        if (!*name)
            exit(EXIT_FAILURE);
        ft_memcpy(*name, env_var, name_len);
        (*name)[name_len] = '\0';
        *value = ft_strdup(equal_sign + 1);
    } else {
        *name = ft_strdup(env_var);
        *value = NULL;
    }
}
t_env_node *copy_envp_to_list(char **envp)
{
	t_env_node *head;
	char *name;
	char *value;
    t_env_node *current;
    int i;
	t_env_node *new_node;

	head = NULL;
	current = NULL;
	i = 0;
    while (envp[i])
	{
        name = NULL;
        value = NULL;

        split_env_var(envp[i], &name, &value);
        new_node = malloc(sizeof(t_env_node));
        if (!new_node)
            exit(EXIT_FAILURE);
        new_node->name = name;
        new_node->value = value;
        new_node->next = NULL;
        if (!head)
            head = new_node;
        else
            current->next = new_node;
        current = new_node;
        i++;
    }
    return head;
}

int	main(int argc, char **argv, char **envp)
{
	t_env_node *envp_list;

	envp_list = copy_envp_to_list(envp);
	(void)argc;
	(void)argv;
	// (void)envp;
	start_bash(envp, envp_list);
	finish_bash();
	// system("leaks minishell");
	// printf("%s\n", getenv("$$"));
	free_env_list(envp_list);
	return (0);
}
