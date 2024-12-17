/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/17 15:23:14 by tkeil            ###   ########.fr       */
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

	i = 0;
	char	*types[] = {[OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
			[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
			[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
			[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
			[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
			[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
			[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
			[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

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

void clr_exec_table(t_exec_table *exec_table)
{
	int i;
	t_lexems *current;
	t_lexems *next;

	i = 0;
	while(exec_table[0].lexems[i])
	{
		current = exec_table[0].lexems[i];
		while(current)
		{
			next = current->next;
			free(current->value);
			free(current);
			current = next;
		}
		i++;
	}
}

void	get_user_input(char **envp)
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
		system("leaks minishell");
		exit(0);
	}
	add_history(prompt);
	create_lexes(&lexems, prompt);
	create_exec_table(&lexems, &exec_table);
	// ft_test_exec_table(exec_table);
	// parse, execute are not there yet
	// parse_lexes(&lexems);
	execute_commands(&exec_table, envp);
	clr_exec_table(&exec_table);
	clr_lexes(&lexems);
	free(text_show);
	free(prompt);
}

void handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void handle_sigquit(int sig) {
	(void)sig;
	rl_redisplay();
}

void configure_terminal(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
		system("leaks minishell");
        exit(EXIT_FAILURE);
    }
    term.c_lflag &= ~ECHOCTL;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("tcsetattr");
		system("leaks minishell");
        exit(EXIT_FAILURE);
    }
}

void start_bash(char **envp)
{
	configure_terminal();
	display_minishell_intro();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	while(1)
	{
		get_user_input(envp);
	}
}

void finish_bash()
{
	rl_clear_history();
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	start_bash(envp);
	finish_bash();
	system("leaks minishell");
	return (0);
}
