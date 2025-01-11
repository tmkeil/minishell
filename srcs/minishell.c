/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 19:43:12 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/11 15:33:14 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_test_exec_table(t_minishell *minishell)
// {
// 	t_lexems	*current;
// 	char		*types[] = {[SEPERATOR] = "SEPERATOR", [OR] = "OR",
// 				[AND] = "AND", [PIPE] = "PIPE",
// 				[WORD] = "WORD", [NUMBER] = "NUMBER", [APPEND] = "APPEND",
// 				[HEREDOC] = "HEREDOC", [ENV_VAR] = "ENV_VAR",
// 				[IN_REDIRECT] = "IN_REDIRECT", [OUT_REDIRECT] = "OUT_REDIRECT",
// 				[INVALID] = "INVALID", [LINEFEED] = "LINEFEED",
// 				[O_BRACKET] = "O_BRACKET", [C_BRACKET] = "C_BRACKET",
// 				[AMPERSAND] = "AMPERSAND", [SINGLE_QUOTE] = "SINGLE_QUOTE",
// 				[DOUBLE_QUOTE] = "DOUBLE_QUOTE"};

// 	printf("Testing table:\n");
// 	for (int i = 0; minishell->table[i]; i++)
// 	{
// 		if (!minishell->table[i])
// 			continue ;
// 		current = minishell->table[i];
// 		while (current)
// 		{
// 			printf("minishell.table[%i].Type: %s, minishell.table[%i].Value: %s\n", i, types[current->type], i, (char *)current->value ? (char *)current->value : (char *)"(null)");
// 			current = current->next;
// 		}
// 	}
// }

void	ft_test_cmd_list(t_cmds *cmds)
{
	int i = 0;
	printf("test\n");
	printf("cmd = %p\n", cmds);
	while (cmds)
	{
		printf("cmd = %p\n", cmds);
		printf("cmd = %s\n", cmds->cmd);
		i = 0;
		if (cmds->args)
		{
			while ((cmds->args)[i])
			{
				printf("arg[%i] = %s\n", i, (cmds->args)[i]);
				i++;
			}
		}
		printf("in file = %s\n", cmds->input_file);
		printf("out file = %s\n", cmds->output_file);
		printf("append = %i\n", cmds->append);
		printf("heredoc = %s\n", cmds->heredoc);
		if (cmds->next)
		{
			printf("\npipe\n\n");
		}
		cmds = cmds->next;
	}
	printf("abc\n");
}

int	ft_handle_input(t_minishell **minishell, char *input)
{
	if (!ft_create_lexes(&(*minishell)->tokens, input, (*minishell)->envs))
		return (0);
	if (!ft_create_command_list(minishell, &(*minishell)->cmds))
		return (0);
	// ft_test_cmd_list((*minishell)->cmds);
	if (!ft_execute_commands(minishell))
		return (0);
	if (!ft_set_exit_status(*minishell))
		return (0);
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
	free(prompt);
	if (!input)
		return (free(input), prompt = NULL, input = NULL, 0);
	add_history(input);
	if (!ft_handle_input(&minishell, input))
		return (free(input), 1);
	return (free(input), input = NULL, prompt = NULL, 1);
}

int	ft_input_loop(t_minishell *minishell)
{
	int	exit_status;

	while (1)
	{
		if (!ft_get_user_input(minishell))
			break ;
		ft_free_next_iteration(&minishell);
	}
	exit_status = minishell->exit_status;
	return (ft_free_shell(&minishell), exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	(void)argv;
	if (argc > 1)
		return (EXIT_FAILURE);
	minishell.in_fd = STDIN_FILENO;
	minishell.out_fd = STDOUT_FILENO;
	minishell.tokens = NULL;
	minishell.table = NULL;
	minishell.envs = NULL;
	minishell.envps = NULL;
	minishell.cmds = NULL;
	minishell.exit_status = 0;
	if (!ft_extract_envps(&minishell.envs, envp)
		|| !ft_update_envps(minishell.envs, &minishell.envps))
		return (ft_free_envs(&minishell.envs), EXIT_FAILURE);
	ft_init_sig();
	ft_set_exit_status(&minishell);
	rl_clear_history();
	return (ft_input_loop(&minishell));
}
