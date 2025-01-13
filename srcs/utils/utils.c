/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:13:21 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 18:19:44 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// void	disable_ctrl_chars(void)
// {
// 	struct termios			new_termios;
// 	static struct termios	orig_termios;

// 	tcgetattr(STDIN_FILENO, &orig_termios);
// 	new_termios = orig_termios;
// 	new_termios.c_lflag &= ~(ECHOCTL);
// 	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
// }

void	ft_init_sig(void)
{
	signal(SIGINT, ft_handle_sig);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_set_execution_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ft_sterr(char *msg1, char *msg2, char *msg3)
{
	if (msg1)
		ft_putstr_fd(msg1, STDERR_FILENO);
	if (msg2)
		ft_putstr_fd(msg2, STDERR_FILENO);
	if (msg3)
		ft_putstr_fd(msg3, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

size_t	ft_ptrsize(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
	{
		i++;
	}
	return (i);
}
