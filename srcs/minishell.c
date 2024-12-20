/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/21 00:41:52 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_set_exit_status(t_minishell **minishell)
{
	return (ft_set_env("?", ft_itoa((*minishell)->exit_status), &(*minishell)->envs));
}

int	ft_get_user_input(char **envp, t_minishell *minishell)
{
	char	*prompt;
	char	*sh;

	sh = ft_strjoin(getenv("USER"), "@minishell $ ");
	if (!sh)
		return (0);
	prompt = readline(sh);
	if (!prompt)
		return (free(sh), 0);
	add_history(prompt);
	if (!ft_create_lexes(&minishell->tokens, prompt))
		return (ft_free_shell(&minishell), free(prompt), free(sh), 0);
	if (!ft_create_exec_table(&minishell))
		return (ft_free_shell(&minishell), free(prompt), free(sh), 0);
	// ft_test_exec_table(*minishell);
	if (!ft_execute_commands(&minishell, envp))
		return (ft_free_shell(&minishell), free(prompt), free(sh), 0);
	if (!ft_set_exit_status(&minishell))
		return (ft_free_shell(&minishell), free(prompt), free(sh), 0);
	ft_free_tokens(&minishell->tokens);
	ft_free_table(&minishell->table);
	return (free(sh), free(prompt), 1);
}

void	ft_start_bash(char **envp)
{
	t_minishell	minishell;

	minishell.tokens = NULL;
	minishell.table = NULL;
	minishell.envs = NULL;
	if (!ft_extract_envps(&minishell.envs, envp))
	{
		ft_free_envs(&minishell.envs);
		// system("leaks minishell");
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, ft_handle_sigquit);
	ft_configure_terminal();
	ft_display_intro();
	while (1)
	{
		if (!ft_get_user_input(envp, &minishell))
		{
			// system("leaks minishell");
			exit(EXIT_FAILURE);
		}
	}
}

void	ft_finish_bash(void)
{
	rl_clear_history();
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	ft_start_bash(envp);
	ft_finish_bash();
	// system("leaks minishell");
	return (0);
}

// void	ft_test_exec_table(t_minishell minishell)
// {
// 	int			i;
// 	t_lexems	*current;
// 	char		*types[] = {[OR] = "OR", [AND] = "AND", [PIPE] = "PIPE",
// 				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
// 				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
// 				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
// 				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
// 				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
// 				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
// 				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

// 	i = 0;
// 	while (minishell.table[i])
// 	{
// 		current = minishell.table[i];
// 		while (current)
// 		{
// 			printf("table.lexems[%i]->type = %s\n", i, types[current->type]);
// 			printf("table.lexems[%i]->value = %s\n", i, (char *)current->value);
// 			current = current->next;
// 		}
// 		i++;
// 	}
// }
