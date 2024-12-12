/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/12 23:50:54 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_user_input(void)
{
	char	*prompt;

	ft_printf("%s@minishell $", getenv("USER"));
	prompt = readline("");
	create_lexes(prompt);
	free(prompt);
}

int main(int argc, char **argv, char **envp)
{
	display_minishell_intro();
	get_user_input();
	return (0);
}
