/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/03 01:52:07 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_test_exec_table(t_minishell minishell)
// {
// 	t_lexems	*current;
// 	char		*types[] = {[SEPERATOR] = "SEPERATOR", [OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
// 				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
// 				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
// 				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
// 				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
// 				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
// 				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
// 				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

// 	printf("Testing table:\n");
// 	for (int i = 0; minishell.table[i]; i++)
// 	{
// 		if (!minishell.table[i])
// 			continue ;
// 		current = minishell.table[i];
// 		while (current)
// 		{
// 			printf("minishell.table[%i].Type: %s, minishell.table[%i].Value: %s\n", i, types[current->type],
// 				i, (char *)current->value ? (char *)current->value : (char *)"(null)");
// 			current = current->next;
// 		}
// 	}
// }

int ft_is_wsl_environment(void)
{
    if (getenv("WSLENV"))
        return (1);
    return (0);
}

int ft_is_interactive(void)
{
    if (isatty(STDIN_FILENO))
		return (1);
	return (ft_is_wsl_environment());
}

int ft_set_exit_status(t_minishell *minishell)
{
    char	*exit_status_str;

    exit_status_str = ft_itoa(minishell->exit_status);
    if (!exit_status_str)
        return (0);
    if (!ft_set_env("?", exit_status_str, &minishell->envs))
        return (free(exit_status_str), 0);
    return (free(exit_status_str), 1);
}

char *ft_get_prompt(void)
{
	char 	*user;
	char	*prompt;
	
	user = getenv("USER");
	prompt = ft_strjoin(user, "@minishell $ ");
	if (!prompt)
		return (ft_strdup("user@minishell $ "));
	return (prompt);
}

char *ft_get_input(const char *prompt)
{
    char *input;

    input = readline(prompt);
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
	if (!ft_set_exit_status(*minishell))
		return (ft_free_shell(minishell), 0);
	return (1);
}

int ft_get_user_input(t_minishell *minishell)
{
    char		*prompt;
    char 		*input;
	char		*line;

	printf("test2\n");
	input = NULL;
	prompt = NULL;
	prompt = ft_get_prompt();
	printf("test3\n");
	if (!prompt)
		return (0);
	printf("test4\n");
	if (ft_is_interactive())
	{
		printf("test5\n");
		input = readline(prompt);
		printf("test55\n");
	}
	else
	{
		printf("test6\n");
		line = get_next_line(STDIN_FILENO);
		input = ft_strndup(line, ft_strchr(line, '\n') - line);
		free(line);
	}
	printf("input = %s\n", input);
	if (!input)
		return (free(prompt), prompt = NULL, 0);
	free(prompt);
	prompt = NULL;
	add_history(input);
	ft_set_execution_sig();
	ft_handle_input(&minishell, input);
    return (free(input), input = NULL, 1);
}

void ft_reset_terminal(void)
{
    struct termios term;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        return ;
    }
    term.c_lflag |= (ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		perror("tcsetattr");
}

int ft_input_loop(t_minishell *minishell)
{
	int	exit_status;

    while (1)
    {
		printf("test\n");
        if (!ft_get_user_input(minishell))
            break ;
		ft_free_tokens(&minishell->tokens);
		ft_free_table(&minishell->table);
		ft_reset_terminal();
		ft_init_sig();
    }
	exit_status = minishell->exit_status;
	ft_free_ptr(&minishell->envps);
    ft_free_shell(&minishell);
    return (exit_status);
}

int main(int argc, char **argv, char **envp)
{
	t_minishell minishell;

	(void)argv;
    if (argc > 1)
		return (EXIT_FAILURE);
    minishell.tokens = NULL;
    minishell.table = NULL;
    minishell.envs = NULL;
    minishell.envps = NULL;
    minishell.exit_status = 0;
	rl_clear_history();
    if (!ft_extract_envps(&minishell.envs, envp) || !ft_update_envps(minishell.envs, &minishell.envps))
		return (ft_free_envs(&minishell.envs), EXIT_FAILURE);
	ft_init_sig();
	ft_display_intro();
	ft_set_exit_status(&minishell);
	rl_clear_history();
    return (ft_input_loop(&minishell));
}
