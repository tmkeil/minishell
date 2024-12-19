/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/19 15:14:18 by tkeil            ###   ########.fr       */
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

int	create_exec_table(t_minishell *minishell)
{
	size_t		i;
	size_t		size;
	t_lexems	*lex;

	i = 0;
	lex = minishell->tokens;
	size = ft_table_size(lex) + 2;
	minishell->table = malloc(sizeof(t_lexems *) * size);
	if (!minishell->table)
		return (0);
	while (i < size)
		minishell->table[i++] = NULL;
	i = 0;
	while (lex)
	{
		if (lex->type != PIPE)
			ft_append_node(&minishell->table[i], lex);
		else
			i++;
		lex = lex->next;
	}
	minishell->table[i + 1] = NULL;
	return (1);
}

void	ft_test_exec_table(t_minishell minishell)
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
	while (minishell.table[i])
	{
		current = minishell.table[i];
		while (current)
		{
			printf("table.lexems[%i]->type = %s\n", i, types[current->type]);
			printf("table.lexems[%i]->value = %s\n", i, (char *)current->value);
			current = current->next;
		}
		i++;
	}
}

void	ft_set_exit_status(t_minishell *minishell)
{
	(void)minishell;
	printf("setting\n");
}

void	get_user_input(char **envp)
{
	t_minishell		minishell;
	char			*prompt;
	char			*text_show;

	text_show = ft_strjoin(getenv("USER"), "@minishell $ ");
	prompt = readline(text_show);
	minishell.tokens = NULL;
	if (!prompt)
	{
		exit(0);
	}
	add_history(prompt);
	create_lexes(&minishell.tokens, prompt);
	create_exec_table(&minishell);
	ft_test_exec_table(minishell);
	// parse, execute are not there yet
	// parse_lexes(&lexems);
	execute_commands(minishell.table, envp);
	if (minishell.exit_status != 0)
		ft_set_exit_status(&minishell);
	clr_exec_table(&minishell.table);
	clr_lexes(&minishell.tokens);
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

void	start_bash(char **envp)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	configure_terminal();
	display_minishell_intro();
	while(1)
	{
		get_user_input(envp);
	}
}

void	finish_bash(void)
{
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	// (void)envp;
	start_bash(envp);
	finish_bash();
	// system("leaks minishell");
	// printf("%s\n", getenv("$$"));
	return (0);
}
