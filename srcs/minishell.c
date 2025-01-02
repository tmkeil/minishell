/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/02 14:47:45 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test_exec_table(t_minishell minishell)
{
	t_lexems	*current;
	char		*types[] = {[SEPERATOR] = "SEPERATOR", [OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	printf("Testing table:\n");
	for (int i = 0; minishell.table[i]; i++)
	{
		if (!minishell.table[i])
			continue ;
		current = minishell.table[i];
		while (current)
		{
			printf("minishell.table[%i].Type: %s, minishell.table[%i].Value: %s\n", i, types[current->type],
				i, (char *)current->value ? (char *)current->value : (char *)"(null)");
			current = current->next;
		}
	}
}

int is_wsl_environment(void)
{
    const char *wsl_indicator = getenv("WSLENV");
    if (wsl_indicator)
        return 1;
    return 0;
}

int is_interactive(void)
{
    if (isatty(STDIN_FILENO))
        return 1;
    if (is_wsl_environment())
    {
        printf("WSL erkannt: Standardannahme ist interaktiver Modus.\n");
        return 1; // Für WSL interaktiven Modus erzwingen
    }
    return 0; // Nicht-interaktiv
}

int ft_set_exit_status(t_minishell **minishell)
{
    char	*exit_status_str;

    exit_status_str = ft_itoa((*minishell)->exit_status);
    if (!exit_status_str)
    {
        ft_put_error_str("Error: Memory allocation failed in ft_itoa", "");
        return (0);
    }
    if (!ft_set_env("?", exit_status_str, &(*minishell)->envs))
    {
        free(exit_status_str);
        return (0);
    }
    free(exit_status_str);
    return (1);
}

char *ft_get_prompt(void)
{
	char *user;
	char *prompt;
	
	user = getenv("USER");
	prompt = ft_strjoin(user, "@minishell $ ");
	if (!prompt)
		ft_put_error_str("Error: Could not allocate memory for prompt", "");
	return (prompt);
}

char *ft_get_input(char *prompt)
{
    char *input;
	
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, ft_handle_sigquit);
    input = readline(prompt);
	if (!input)
	{
		write(1, "\n", 1);
		exit(EXIT_SUCCESS);
	}
	return (input);
}

int	ft_handle_input(t_minishell **minishell, char *input)
{
	if (!ft_create_lexes(&(*minishell)->tokens, input, (*minishell)->envs))
		return (0);
	if (!ft_create_exec_table(minishell))
		return (0);
	if (!ft_execute_commands(minishell))
		return (0);
	if (!ft_set_exit_status(minishell))
		return (ft_free_shell(minishell), 0);
	ft_free_tokens(&(*minishell)->tokens);
	ft_free_table(&(*minishell)->table);;
	return (1);
}

int	ft_get_user_input(t_minishell *minishell)
{
	char	*prompt;
	char	*input;

	if (is_interactive())
	{
		printf("isatty\n");
		prompt = ft_get_prompt();
		if (!prompt)
			return (-1);
		input = ft_get_input(prompt);
		free(prompt);
	}
	else
	{
		printf("else\n");
		input = get_next_line(STDIN_FILENO);
		if (!input || *input == '\0')
		{
			printf("no in\n");
			return (free(input), 0);
		}
	}
	add_history(input);
	if (!ft_handle_input(&minishell, input))
	{
		printf("is 0\n");
		return (free(input), 1);
	}
	return (free(input), 1);
}

int	ft_start_bash(char **envp)
{
	t_minishell	minishell;

	minishell.tokens = NULL;
	minishell.table = NULL;
	minishell.envs = NULL;
	minishell.envps = NULL;
	minishell.exit_status = 0;
	if (!ft_extract_envps(&minishell.envs, envp)
		|| !ft_update_envps(minishell.envs, &minishell.envps))
	{
		ft_free_envs(&minishell.envs);
		exit(EXIT_FAILURE);
	}
	ft_configure_terminal();
	ft_display_intro();
	ft_init_sig();
	while (1)
	{
		if (!ft_get_user_input(&minishell))
		{
			printf("beendet\n");
			if (!isatty(STDIN_FILENO))
				break ;
		}
	}
	return (minishell.exit_status);
}

void	ft_finish_bash(void)
{
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	int	return_argument;

	(void)argc;
	(void)argv;
	return_argument = ft_start_bash(envp);
	ft_finish_bash();
	// system("leaks minishell");
	return (return_argument);
}
