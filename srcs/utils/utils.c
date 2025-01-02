/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:13:21 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/02 17:26:26 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void ft_init_sig(void)
{
    signal(SIGINT, ft_handle_sigint);
    signal(SIGQUIT, ft_handle_sigquit);
}

void	ft_handle_sigquit(int sig)
{
	(void)sig;
	// write(1, "\nCaught SIGQUIT. Ignored.\n", 26);
	// rl_redisplay();
}

void ft_set_execution_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void	ft_configure_terminal(void)
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

void	ft_display_intro(void)
{
	printf("\n");
	printf("  ____  _          _ _   \n");
	printf(" / ___|| |__   ___| | |  \n");
	printf(" \\___ \\| '_ \\ / _ \\ | |  \n");
	printf("  ___) | | | |  __/ | |  \n");
	printf(" |____/|_| |_|\\___|_|_|  \n");
	printf("\n");
}

void	ft_put_error_str(char *msg, char *value)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putendl_fd(value, STDERR_FILENO);
}
