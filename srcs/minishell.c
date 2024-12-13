/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/13 17:46:57 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_user_input(void)
{
	char	*prompt;
	char	*text_show;

	text_show = ft_strjoin(getenv("USER"), "@minishell $ ");
	prompt = readline(text_show);
	add_history(prompt);
	create_lexes(prompt);
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

void start_bash()
{
	display_minishell_intro();
	signal(SIGINT, handle_sigint);
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
