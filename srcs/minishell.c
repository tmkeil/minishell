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
	prompt = readline(" ");
	create_lexes(prompt);
	free(prompt);
}

void start_bash()
{
	display_minishell_intro();
	while(1)
	{
		get_user_input();
	}
}

int main(void)
{
	start_bash();
	return (0);
}
