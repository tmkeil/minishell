/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 20:22:00 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_size(t_lexems *lexes)
{
	size_t	i;

	i = 0;
	while (lexes)
	{
		if (lexes->type != PIPE)
			i++;
		lexes = lexes->next;
	}
	return (i);
}

void	ft_execute(t_lexems *lexems, char *cmd, char **envp, t_env_node *envp_list)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (ft_size(lexems) + 1));
	if (!args)
		return ;
	i = 0;
	while (lexems)
	{
		args[i] = NULL;
		handle_lexem(args, i++, (char *)lexems->value, lexems->type, envp_list);
		lexems = lexems->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
		clean_args(args);
	clean_args(args);
}

int	ft_check_builtin(t_lexems *lexems, char **envp, t_env_node *envp_list)
{
	if (ft_changedir(lexems))
		return (1);
	if (ft_handle_export(lexems, envp_list))
		return (1);
	if (ft_unset(lexems, envp))
		return (1);
	return (0);
}

int	execute_commands(t_exec_table *exec_table, char **envp, t_env_node *envp_list)
{
	int		i;
	char	*cmd;
	int		pid;
	bool	valid;

	i = 0;
	pid = 1;
	cmd = NULL;
	while (exec_table->lexems[i])
	{
		valid = false;
		if (ft_check_builtin(exec_table->lexems[i], envp, envp_list))
		{
			i++;
			continue ;
		}
		cmd = ft_getpath(exec_table->lexems[i]->value, envp);
		if (cmd)
		{
			pid = fork();
			if (pid == 0)
				ft_execute(exec_table->lexems[i], cmd, envp, envp_list);
			valid = true;
		}
		if (pid > 0)
		{
			i++;
			free(cmd);
			waitpid(pid, NULL, 0);
		}
		if (!valid)
			printf("zsh: command not found: %s\n", (char *)exec_table->lexems[i - 1]->value);
	}
	return (2);
}
