/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/16 21:01:03 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_table_size(t_lexems *lexems)
{
	size_t	size;

	size = 0;
	if (!lexems)
		return (0);
	while (lexems->next)
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

	if (!*table)
	{
		*table = lex;
		return ;
	}
	last = *table;
	while (last->next)
		last = last->next;
	last->next = lex;
	return ;
}

int	create_exec_table(t_lexems **lexems, t_exec_table *exec_table)
{
	size_t		i;
	size_t		size;
	t_lexems	*lex;

	i = 0;
	lex = *lexems;
	size = ft_table_size(lex);
	exec_table->lexems = malloc(sizeof(t_lexems *) * (size + 1));
	if (!exec_table->lexems)
		return (0);
	while (lex->next)
	{
		if (lex->type != PIPE)
			ft_append_node(&exec_table->lexems[i], lex);
		else
			i++;
		lex = lex->next;
	}
	return (1);
}

void	get_user_input(void)
{
	t_lexems		*lexems;
	t_exec_table	exec_table;
	char			*prompt;
	char			*text_show;

	lexems = NULL;
	text_show = ft_strjoin(getenv("USER"), "@minishell $ ");
	prompt = readline(text_show);
	if (!prompt)
		exit(0);
	add_history(prompt);
	create_lexes(&lexems, prompt);
	create_exec_table(&lexems, &exec_table);
	// parse, execute are not there yet
	// parse_lexes(&lexems);
	// execute_commands();
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
        exit(EXIT_FAILURE);
    }
    term.c_lflag &= ~ECHOCTL;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void start_bash()
{
	configure_terminal();
	display_minishell_intro();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	while(1)
	{
		get_user_input();
	}
}

void finish_bash()
{
	rl_clear_history();
}

int main(void)
{
	start_bash();
	finish_bash();
	return (0);
}
