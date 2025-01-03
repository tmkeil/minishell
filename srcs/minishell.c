/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 23:43:26 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test_exec_table(t_minishell *minishell)
{
	t_lexems	*current;
	char		*types[] = {[SEPERATOR] = "SEPERATOR", [OR] = "OR",
				[AND] = "AND", [PIPE] = "PIPE",
				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	printf("Testing table:\n");
	for (int i = 0; minishell->table[i]; i++)
	{
		if (!minishell->table[i])
			continue ;
		current = minishell->table[i];
		while (current)
		{
			printf("minishell.table[%i].Type: %s, minishell.table[%i].Value: %s\n", i, types[current->type], i, (char *)current->value ? (char *)current->value : (char *)"(null)");
			current = current->next;
		}
	}
}


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

int	ft_handle_input(t_minishell **minishell, char *input)
{
	if (!ft_create_lexes(&(*minishell)->tokens, input, (*minishell)->envs))
		return (0);
	if (!ft_create_exec_table(minishell))
		return (0);
	ft_test_exec_table(*minishell);
	if (!ft_execute_commands(minishell))
		return (0);
	if (!ft_set_exit_status(*minishell))
		return (ft_free_shell(minishell), 0);
	return (1);
}

int	ft_get_user_input(t_minishell *minishell)
{
	char	*prompt;
	char	*input;
	char	*line;

	prompt = ft_get_prompt();
	if (!prompt)
		return (0);
	if (ft_is_interactive())
		input = readline(prompt);
	else
	{
		line = get_next_line(fileno(stdin));
		input = ft_strtrim(line, "\n");
		free(line);
	}
	if (!input || *input == '\0')
		return (free(prompt), free(input), prompt = NULL, input = NULL, 0);
	free(prompt);
	if (input)
	{
		add_history(input);
		ft_handle_input(&minishell, input);
	}
	return (free(input), input = NULL, prompt = NULL, 1);
}

int	ft_input_loop(t_minishell *minishell)
{
	int	exit_status;

	while (1)
	{
		if (!ft_get_user_input(minishell))
			break ;
		ft_free_tokens(&minishell->tokens);
		ft_free_table(&minishell->table);
	}
	exit_status = minishell->exit_status;
	ft_free_ptr(&minishell->envps);
	ft_free_ptr(&minishell->cached_envps);
	ft_free_shell(&minishell);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argv;
	if (argc > 1)
		return (EXIT_FAILURE);
	disable_ctrl_chars();
	minishell.tokens = NULL;
	minishell.table = NULL;
	minishell.envs = NULL;
	minishell.envps = NULL;
	minishell.cached_envps = NULL;
	minishell.exit_status = 0;
	if (!ft_extract_envps(&minishell.envs, envp)
		|| !ft_update_envps(minishell.envs, &minishell.envps))
		return (ft_free_envs(&minishell.envs), EXIT_FAILURE);
	ft_init_sig();
	ft_set_exit_status(&minishell);
	rl_clear_history();
	return (ft_input_loop(&minishell));
}
