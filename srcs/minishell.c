/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/16 16:29:34 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_user_input(void)
{
	t_lexems	*lexems;
	char		*prompt;
	char		*text_show;

	lexems = NULL;
	text_show = ft_strjoin(getenv("USER"), "@minishell $ ");
	prompt = readline(text_show);
	if (!prompt)
		exit(0);
	add_history(prompt);
	create_lexes(&lexems, prompt);
	// parse, execute are not there yet
	parse_lexes(&lexems);
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
