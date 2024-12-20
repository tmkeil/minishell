/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 13:22:10 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_test_exec_table(t_minishell minishell)
{
	int			i;
	t_lexems	*current;
	char		*types[] = {[OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

	i = 0;
	while (minishell.table[i])
	{
		current = minishell.table[i];
		while (current)
		{
			printf("table.lexems[%i]->type = %s\n", i, types[current->type]);
			printf("table.lexems[%i]->value = %s\n", i, (char *)current->value);
			current = current->next;
		}
		i++;
	}
}

void	ft_set_exit_status(t_minishell *minishell)
{
	(void)minishell;
	// printf("setting\n");
}

void	get_user_input(char **envp)
{
	t_minishell	minishell;
	char		*prompt;
	char		*text_show;

	text_show = ft_strjoin(getenv("USER"), "@minishell $ ");
	prompt = readline(text_show);
	minishell.tokens = NULL;
	if (!prompt)
		exit(0);
	extract_envps(&minishell.envs, envp);
	add_history(prompt);
	create_lexes(&minishell.tokens, prompt);
	create_exec_table(&minishell);
	// ft_test_exec_table(minishell);
	// parse, execute are not there yet
	// parse_lexes(&lexems);
	execute_commands(minishell.table, envp, minishell.envs);
	if (minishell.exit_status != 0)
		ft_set_exit_status(&minishell);
	clr_exec_table(&minishell.table);
	clr_lexes(&minishell.tokens);
	free_env_list(&minishell.envs);
	free(text_show);
	free(prompt);
}

void	start_bash(char **envp)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	configure_terminal();
	display_minishell_intro();
	while (1)
	{
		get_user_input(envp);
	}
}

void	finish_bash(void)
{
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	// (void)envp;
	start_bash(envp);
	finish_bash();
	// system("leaks minishell");
	// printf("%s\n", getenv("$$"));
	return (0);
}
