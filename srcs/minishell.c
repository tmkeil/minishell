/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/14 12:42:28 by tkeil            ###   ########.fr       */
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
	execute_commands();
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
}

void start_bash()
{
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
