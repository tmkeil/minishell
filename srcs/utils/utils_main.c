/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:50:10 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/05 13:50:30 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_wsl_environment(void)
{
	if (getenv("WSLENV"))
		return (1);
	return (0);
}

int	ft_is_interactive(void)
{
	if (isatty(fileno(stdin)))
		return (1);
	return (ft_is_wsl_environment());
}

int	ft_set_exit_status(t_minishell *minishell)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(minishell->exit_status);
	if (!exit_status_str)
		return (0);
	if (!ft_set_env("?", exit_status_str, &minishell->envs))
		return (free(exit_status_str), 0);
	return (free(exit_status_str), 1);
}

char	*ft_get_prompt(void)
{
	char	*user;
	char	*prompt;

	user = getenv("USER");
	prompt = ft_strjoin(user, "@minishell $ ");
	if (!prompt)
		return (ft_strdup("user@minishell $ "));
	return (prompt);
}

char	*ft_get_input(const char *prompt)
{
	char	*input;

	input = readline(prompt);
	return (input);
}
